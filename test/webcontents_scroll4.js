const path = require("path");
const { app, BrowserView, BaseWindow, ContainerView, ScrollView } = require("electron");

const APP_WIDTH = 600;
const GAP = 30;

const APPS = [
  "https://bitbucket.org",
  "https://github.com",
  "https://youtube.com",
  "https://figma.com/",
  "https://thenextweb.com/",
  "https://engadget.com/",
  "https://theverge.com/",
  "https://mashable.com",
  "https://www.businessinsider.com",
  "https://wired.com",
  "https://macrumors.com",
  "https://gizmodo.com",
  "https://maps.google.com/",
  "https://sheets.google.com/",
];

global.win = null;

function createWindow () {
  // Create window.
  win = new BaseWindow({ autoHideMenuBar: true, width: 1400, height: 700 });
  //win.setContentSize({ width: 600, height: 500 });

  win.setUseYoga(true);

  // The content view.
  const contentView = new ContainerView();
  //contentView.setStyle({ flexDirection: "row", backgroundColor: "#1F2937" });
  contentView.setStringProperty("flexDirection", "row");
  contentView.setBackgroundColor("#1F2937");
  contentView.layout();

  win.setContainerView(contentView);

  // Scroll
  const scroll = new ScrollView();
  //scroll.setStyle({
    //flex: 1,
  //});
  scroll.setNumericProperty("flex", 1);
  scroll.layout();
  scroll.setHorizontalScrollBarMode("enabled");
  scroll.setVerticalScrollBarMode("disabled");

  contentView.addContainerView(scroll);

  // Scroll content
  const scrollContent = new ContainerView();
  //scrollContent.setStyle({
    //flexDirection: "row",
    //flex: 1,
    //padding: 30,
    //flexShrink: 0,
  //});
  scrollContent.setStringProperty("flexDirection", "row");
  scrollContent.setNumericProperty("flex", 1);
  scrollContent.setNumericProperty("padding", 30);
  scrollContent.setNumericProperty("flexShrink", 0);
  scrollContent.setBounds({x: 0, y: 0, width: APPS.length * (APP_WIDTH + GAP), height: 600});
  scrollContent.layout();
  scrollContent.setBackgroundColor("#FF0000");
  scroll.setContentView(scrollContent);

  // Webview
  const addWebview = function (scrollContent, url, i) {
    //chrome.setStyle({ flex: 1 });
    //const webContentView = gui.Container.create();
    //webContentView.setStyle({
      //width: APP_WIDTH,
      //height: "100%",
      //backgroundColor: "#ffffff",
      //marginRight: GAP,
    //});
    //webContentView.addChildView(chrome);
    const browserView = new BrowserView();
    browserView.webContents.loadURL(url);
    browserView.setBackgroundColor("#ffffff");
    const webContentView = new ContainerView();
    webContentView.setNumericProperty("width", APP_WIDTH);
    webContentView.setStringProperty("height", "100%");
    webContentView.setStringProperty("backgroundColor", "#ffffff");
    webContentView.setNumericProperty("marginRight", GAP);
    webContentView.layout();
    webContentView.addBrowserView(browserView);
    scrollContent.addContainerView(webContentView);
  };

  var i = 0;
  APPS.forEach((app) => {
    addWebview(scrollContent, app, i++);
  });

  //contentView.layout();
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow();

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BaseWindow.getAllWindows().length === 0)
      createWindow();
  });
});

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin')
    app.quit();
});
