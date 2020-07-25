struct Presentation : Window {
  enum : uint { StatusHeight = 24 };

  Presentation();
  auto resizeWindow() -> void;
  auto loadEmulators() -> void;
  auto loadEmulator() -> void;
  auto unloadEmulator(bool reloading = false) -> void;
  auto showIcon(bool visible) -> void;
  auto loadShaders() -> void;

  MenuBar menuBar{this};
    Menu loadMenu{&menuBar};
    Menu systemMenu{&menuBar};
    Menu settingsMenu{&menuBar};
      Menu videoSizeMenu{&settingsMenu};
      Menu videoOutputMenu{&settingsMenu};
        MenuRadioItem videoOutputCenter{&videoOutputMenu};
        MenuRadioItem videoOutputScale{&videoOutputMenu};
        MenuRadioItem videoOutputStretch{&videoOutputMenu};
        Group videoOutputGroup{&videoOutputCenter, &videoOutputScale, &videoOutputStretch};
        MenuSeparator videoOutputSeparator{&videoOutputMenu};
        MenuCheckItem videoAspectCorrection{&videoOutputMenu};
        MenuCheckItem videoAdaptiveSizing{&videoOutputMenu};
        MenuCheckItem videoAutoCentering{&videoOutputMenu};
      Menu videoShaderMenu{&settingsMenu};
      MenuSeparator videoSettingsSeparatpr{&settingsMenu};
      MenuCheckItem muteAudioSetting{&settingsMenu};
      MenuCheckItem showStatusBarSetting{&settingsMenu};
      MenuSeparator audioSettingsSeparator{&settingsMenu};
      MenuItem videoSettingsAction{&settingsMenu};
      MenuItem audioSettingsAction{&settingsMenu};
      MenuItem inputSettingsAction{&settingsMenu};
      MenuItem hotkeySettingsAction{&settingsMenu};
      MenuItem emulatorSettingsAction{&settingsMenu};
      MenuItem firmwareSettingsAction{&settingsMenu};
      MenuItem pathSettingsAction{&settingsMenu};
      MenuItem driverSettingsAction{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveStateMenu{&toolsMenu};
      Menu loadStateMenu{&toolsMenu};
      MenuSeparator stateMenuSeparator{&toolsMenu};
      MenuCheckItem pauseEmulation{&toolsMenu};
      MenuSeparator toolsMenuSeparator{&toolsMenu};
      MenuItem manifestViewerAction{&toolsMenu};
      MenuItem traceLoggerAction{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem webpageAction{&helpMenu};
      MenuSeparator aboutSeparator{&helpMenu};
      MenuItem aboutAction{&helpMenu};

  VerticalLayout layout{this};
    HorizontalLayout viewportLayout{&layout, Size{~0, ~0}, 0};
      Viewport viewport{&viewportLayout, Size{~0, ~0}, 0};
      VerticalLayout iconLayout{&viewportLayout, Size{0, ~0}, 0};
        Canvas iconSpacer{&iconLayout, Size{128, ~0}, 0};
        Canvas iconCanvas{&iconLayout, Size{128, 128}, 0};
    HorizontalLayout statusLayout{&layout, Size{~0, StatusHeight}, 0};
      Label spacerLeft{&statusLayout, Size{8, ~0}, 0};
      Label statusLeft{&statusLayout, Size{~0, ~0}, 0};
      Label statusRight{&statusLayout, Size{100, ~0}, 0};
      Label spacerRight{&statusLayout, Size{8, ~0}, 0};
};

namespace Instances { extern Instance<Presentation> presentation; }
extern Presentation& presentation;
