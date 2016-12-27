#
# Copyright 2016 by Shaheed Haque (srhaque@theiet.org)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301  USA.
#
"""
SIP binding customisation for PyKF5.KWidgetAddons. This modules describes:

    * The SIP file generator rules.
"""

import os, sys

import rules_engine
sys.path.append(os.path.dirname(os.path.dirname(rules_engine.__file__)))
import Qt5Ruleset

from clang.cindex import AccessSpecifier, CursorKind

def local_container_rules():
    return [
        [".*", "KMessageBox.*", ".*", ".*", ".*", rules_engine.container_discard],
        [".*", "KMultiTabBar.*", ".*", ".*", ".*", rules_engine.container_discard],
    ]

def local_function_rules():
    return [
        [".*", ".*", ".*", ".*", ".*KConfig.*", rules_engine.function_discard],

        # How should Qt::CheckState *sizeIsRelativeState be handled?
        ["KFontChooser", "KFontChooser", ".*", ".*", ".*", rules_engine.function_discard],

        # Needs MappedType for QPair<KGuiItem, KGuiItem>
        ["KStandardGuiItem", "backAndForward", ".*", ".*", ".*", rules_engine.function_discard],

        ["KRatingWidget", "ratingChanged", ".*", ".*", ".*unsigned int.*", rules_engine.function_discard],
        ["KRatingWidget", "setRating", ".*", ".*", ".*unsigned int.*", rules_engine.function_discard],
        ["KRatingWidget", "setMaxRating", ".*", ".*", ".*unsigned int.*", rules_engine.function_discard],

        ["KPageDialog", "pageWidget", ".*", "const KPageWidget.*", ".*", rules_engine.function_discard],
        ["KPageDialog", "buttonBox", ".*", "const QDialogButtonBox.*", ".*", rules_engine.function_discard],
    ]

def qualify_enum_initialiser(container, function, parameter, sip, matcher):
    """Enums in initialisers need to be fully qualified."""
    sip["init"] = rules_engine._parents(function) + "::" + sip["init"]

def local_parameter_rules():
    return [
        ["KStandardGuiItem", "back|forward", "useBidi", ".*", ".*", qualify_enum_initialiser],
    ]

class RuleSet(Qt5Ruleset.RuleSet):
    """
    SIP file generator rules. This is a set of (short, non-public) functions
    and regular expression-based matching rules.
    """
    def __init__(self, includes):
        Qt5Ruleset.RuleSet.__init__(self, includes)
        self._param_db = rules_engine.ParameterRuleDb(lambda: local_parameter_rules() + Qt5Ruleset.parameter_rules())
        self._fn_db = rules_engine.FunctionRuleDb(lambda: local_function_rules() + Qt5Ruleset.function_rules())
        self._container_db = rules_engine.ContainerRuleDb(lambda: local_container_rules() + Qt5Ruleset.container_rules())
