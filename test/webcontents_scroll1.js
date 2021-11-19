const { app, BoxLayout, BrowserView, BaseWindow, ContainerView, ScrollView } = require('electron')

// Note for Mac when using BrowserWindow:
// When using `views::Widget` to hold WebContents, Chromium would use
// `BridgedContentView` as content view, which does not support draggable
// regions. In order to make draggable regions work, we have to replace the
// content view with a simple NSView.
// in result we cannot use views on Mac and having support for draggable regions at the same time.

 app.whenReady().then(() => {
  const win = new BaseWindow({ autoHideMenuBar: true, width: 1100, height: 950 })

  const content_scroll = new ContainerView()
  const layout = new BoxLayout({ orientation: "horizontal" })
  content_scroll.setBoxLayout(layout)
  content_scroll.setBounds({ x: 0, y: 0, width: 3500, height: 900 });

  const browser_view1 = new BrowserView()
  browser_view1.setPreferredSize({ width: 1000, height: 800 })
  content_scroll.addBrowserView(browser_view1)
  browser_view1.webContents.loadURL('https://bitbucket.org')

  const browser_view2 = new BrowserView()
  browser_view2.setPreferredSize({ width: 1000, height: 800 })
  content_scroll.addBrowserView(browser_view2)
  browser_view2.webContents.loadURL('https://github.com')

  const browser_view3 = new BrowserView()
  browser_view3.setPreferredSize({ width: 1000, height: 800 });
  content_scroll.addBrowserView(browser_view3)
  browser_view3.webContents.loadURL('https://youtube.com')

  const scroll = new ScrollView()
  scroll.setContentView(content_scroll)

  //win.setContainerView(scroll)

  const content_view = new ContainerView()
  const content_layout = new BoxLayout({ orientation: "vertical" })
  content_view.setBoxLayout(content_layout)
  content_view.addContainerView(scroll)

  win.setContainerView(content_view)
}) 
