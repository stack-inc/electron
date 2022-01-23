const { app, BrowserView, BaseWindow } = require('electron')

// Note for Mac when using BrowserWindow:
// When using `views::Widget` to hold WebContents, Chromium would use
// `BridgedContentView` as content view, which does not support draggable
// regions. In order to make draggable regions work, we have to replace the
// content view with a simple NSView.
// in result we cannot use views on Mac and having support for draggable regions at the same time.

 app.whenReady().then(() => {
  const win = new BaseWindow({ width: 1400, height: 600 })

  win.setContentExampleView()
}) 
