const path = require("path");
const { app, BoxLayout, BrowserView, BaseWindow, ContainerView, ScrollView } = require("electron");

const APP_WIDTH = 1000;
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
  win = new BaseWindow({ autoHideMenuBar: true, width: 1100, height: 950 })
  //win.setContentSize({ width: 600, height: 500 });

  // The content view.
  const content_view = new ContainerView();
  //content_view.setStyle({ flexDirection: "row", backgroundColor: "#1F2937" });
  const contentLayout = new BoxLayout({ orientation: "vertical" })
  content_view.setBoxLayout(contentLayout);
  content_view.setBackgroundColor("#1F2937");

  win.setContainerView(content_view);

  // Scroll
  const scroll = new ScrollView();
  //scroll.setStyle({
    //flex: 1,
  //});
  //scroll.setScrollbarPolicy("automatic", "never")
  content_view.addContainerView(scroll);

  // Scroll content
  const scrollContent = new ContainerView();
  //scrollContent.setStyle({
    //flexDirection: "row",
    //flex: 1,
    //padding: 30,
    //flexShrink: 0,
  //});
  const scrollLayout = new BoxLayout({ orientation: "horizontal", insideBorderInsets: {"top": 20, "left": 20, "bottom": 20, "right": 20}, "betweenChildSpacing": 20 })
  scrollContent.setBoxLayout(scrollLayout);
  //scroll.setContentSize({ width: APPS.length * (APP_WIDTH + GAP) });
scrollContent.setBackgroundColor("#FF0000");
  scrollContent.setBounds({ x: 0, y: 0, width: APPS.length * (APP_WIDTH + GAP), height: 900 });
  scroll.setContentView(scrollContent);

  // Webview
  const addWebview = function (scrollContent, url) {
    //chrome.setStyle({ flex: 1 });
    //const webContentView = gui.Container.create();
    //webContentView.setStyle({
      //width: APP_WIDTH,
      //height: "100%",
      //backgroundColor: "#ffffff",
      //marginRight: GAP,
    //});
    //webContentView.addChildView(chrome);
    const browser_view = new BrowserView();
    browser_view.setPreferredSize({ width: APP_WIDTH, height: 800 });
    browser_view.setBackgroundColor("#ffffff");
    scrollContent.addBrowserView(browser_view);
    browser_view.webContents.loadURL(url);
  };

  APPS.forEach((app) => {
    addWebview(scrollContent, app);
  });
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
