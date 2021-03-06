
ecm_create_qm_loader(kwidgetsaddons_QM_LOADER kwidgetsaddons5_qt)

qt5_add_resources(kwidgetsaddons_ICON_SRCS icons.qrc)

set(kwidgetsaddons_SRCS
  kacceleratormanager.cpp
  kanimatedbutton.cpp
  kcharselect.cpp
  kcharselectdata.cpp
  kcollapsiblegroupbox.cpp
  kcolorbutton.cpp
  kcolumnresizer.cpp
  kdatecombobox.cpp
  kdatepicker.cpp
  kdatetable.cpp
  kdatetimeedit.cpp
  kfontaction.cpp
  knewpasswordwidget.cpp
  fonthelpers.cpp
  kfontsizeaction.cpp
  kpopupframe.cpp
  kselectaction.cpp
  ktimecombobox.cpp
  kdragwidgetdecorator.cpp
  kdualaction.cpp
  kfontchooser.cpp
  kguiitem.cpp
  kled.cpp
  kmessagebox.cpp
  kmessagebox_p.cpp
  kmultitabbar.cpp
  kstandardguiitem.cpp
  kurllabel.cpp # Not good enough quality. Needs to use QUrl instead of QString and should not inherit QLabel, but hold it as a member instead.
  kcapacitybar.cpp
  kfontrequester.cpp
  kpassworddialog.cpp
  kruler.cpp
  kselector.cpp
  kxyselector.cpp
  kseparator.cpp
  ksqueezedtextlabel.cpp
  ktitlewidget.cpp
  ktoggleaction.cpp
  ktogglefullscreenaction.cpp
  kviewstateserializer.cpp
  kviewstatemaintainerbase.cpp
  keditlistwidget.cpp
  kcursor.cpp
  kratingwidget.cpp
  kratingpainter.cpp
  kactionselector.cpp
  kcolorcombo.cpp
  ktoolbarspaceraction.cpp
  kpixmapregionselectordialog.cpp
  kpixmapregionselectorwidget.cpp
  kactionmenu.cpp
  ktoolbarlabelaction.cpp
  ktoolbarpopupaction.cpp
  kpagedialog.cpp
  kpagemodel.cpp
  kpageview.cpp
  kpageview_p.cpp
  kpagewidget.cpp
  kpagewidgetmodel.cpp
  kassistantdialog.cpp
  kmessagewidget.cpp
  knewpassworddialog.cpp
  lineediturldropeventfilter.cpp
  kpixmapsequence.cpp
  kpixmapsequenceoverlaypainter.cpp
  kpixmapsequencewidget.cpp
  kmimetypechooser.cpp
  kmimetypeeditor.cpp
  ksplittercollapserbutton.cpp
  kstyleextensions.cpp
  ${kwidgetsaddons_QM_LOADER}
  ${kwidgetsaddons_ICON_SRCS}
)

set(kwidgetsaddon_UI
   kdatetimeedit.ui
   knewpassworddialog.ui
   knewpasswordwidget.ui
   kpassworddialog.ui
)

qt5_wrap_ui(kwidgetsaddons_SRCS ${kwidgetsaddon_UI} )

add_library(KF5WidgetsAddons ${kwidgetsaddons_SRCS})
generate_export_header(KF5WidgetsAddons BASE_NAME KWidgetsAddons)
add_library(KF5::WidgetsAddons ALIAS KF5WidgetsAddons)

target_link_libraries(KF5WidgetsAddons PUBLIC Qt5::Widgets)

target_include_directories(KF5WidgetsAddons INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KWidgetsAddons>")

set_target_properties(KF5WidgetsAddons PROPERTIES VERSION   ${KWIDGETSADDONS_VERSION_STRING}
                                                  SOVERSION ${KWIDGETSADDONS_SOVERSION}
                                                  EXPORT_NAME WidgetsAddons
)

ecm_generate_headers(KWidgetsAddons_HEADERS
  HEADER_NAMES
  KAcceleratorManager
  KAnimatedButton
  KCharSelect
  KCollapsibleGroupBox
  KColorButton
  KColumnResizer
  KDateComboBox
  KDatePicker
  KDateTimeEdit
  KDragWidgetDecorator
  KDualAction
  KFontAction
  KFontChooser
  KFontSizeAction
  KGuiItem
  KLed
  KMessageBox
  KMessageBoxDontAskAgainInterface
  KMultiTabBar
  KNewPasswordWidget
  KPopupFrame
  KSelectAction
  KStandardGuiItem
  KTimeComboBox
  KUrlLabel
  KCapacityBar
  KFontRequester
  KPasswordDialog
  KRuler
  KSelector
  KTitleWidget
  KXYSelector
  KSeparator
  KSqueezedTextLabel
  KToggleAction
  KToggleFullScreenAction
  KViewStateSerializer
  KViewStateMaintainerBase
  KEditListWidget
  KCursor
  KRatingPainter
  KRatingWidget
  KActionSelector
  KColorCombo
  KActionMenu
  KToolBarLabelAction
  KToolBarPopupAction
  KToolBarSpacerAction
  KPageDialog
  KPageModel
  KPageView
  KPageWidget
  KPageWidgetModel
  KAssistantDialog
  KMessageWidget
  KNewPasswordDialog
  LineEditUrlDropEventFilter
  KPixmapSequence
  KPixmapSequenceOverlayPainter
  KPixmapSequenceWidget
  KPixmapRegionSelectorDialog
  KPixmapRegionSelectorWidget
  KMimeTypeChooser
  KMimeTypeEditor
  KMessageBoxNotifyInterface
  KSplitterCollapserButton
  KStyleExtensions
  REQUIRED_HEADERS KWidgetsAddons_HEADERS
)

find_package(PythonModuleGeneration)

if (PythonModuleGeneration_FOUND)
  ecm_generate_python_binding(
    TARGET KF5::WidgetsAddons
    PYTHONNAMESPACE PyKF5
    MODULENAME KWidgetsAddons
    RULES_FILE "${CMAKE_SOURCE_DIR}/cmake/rules_PyKF5.py"
    SIP_DEPENDS
      QtWidgets/QtWidgetsmod.sip
    HEADERS
      ${KWidgetsAddons_HEADERS}
  )
endif()

install(TARGETS KF5WidgetsAddons EXPORT KF5WidgetsAddonsTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
  ${KWidgetsAddons_HEADERS}
  ${CMAKE_CURRENT_BINARY_DIR}/kwidgetsaddons_export.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KWidgetsAddons COMPONENT Devel
)

install(FILES kcharselect-data  DESTINATION  ${KDE_INSTALL_DATADIR_KF5}/kcharselect)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KWidgetsAddons LIB_NAME KF5WidgetsAddons DEPS "widgets" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KWidgetsAddons)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
