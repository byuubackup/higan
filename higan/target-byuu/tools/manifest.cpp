auto ManifestViewer::construct() -> void {
  setCollapsible();
  setVisible(false);

  manifestLabel.setText("Manifest").setFont(Font().setBold());
  manifestView.setEditable(false).setFont(Font().setFamily(Font::Mono));
};

auto ManifestViewer::reload() -> void {
  string manifest;

  for(auto peripheral : higan::Node::enumerate<higan::Node::Peripheral>(emulator->root)) {
    manifest.append("[", peripheral->name(), "]\n");
    manifest.append(peripheral->manifest(), "\n");
  }
  manifest.trimRight("\n").append("\n");  //ensure only one new line at the end of the manifest

  manifestView.setText(manifest);
}
