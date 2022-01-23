# WrapperBrowserView

A `WrapperBrowserView` is the wrapper for [`BrowserView`](browser-view.md).
It extends [`BaseView`](base-view.md).

## Class: WrapperBrowserView extends `BaseView`

> Wraps 'BrowserView'.

Process: [Main](../glossary.md#main-process)

### Example

```javascript
// In the main process.
const { WrapperBrowserView, BrowserWindow } = require('electron')

const win = new BrowserWindow({ width: 800, height: 600 })

const view = new WrapperBrowserView()
win.addChildView(view)
view.setBounds({ x: 0, y: 0, width: 300, height: 300 })
```

### `new WrapperBrowserView([options])` _Experimental_

* `options` Object (optional)
  * `browserView` [BrowserView](browser-view.md) (optional) - 

If `browserView` is not set then new `BrowserView` is created.

### Instance Properties

Objects created with `new WrapperBrowserView` have the following properties:

#### `view.browserView` _Experimental_

A [`BrowserView`](browser-view.md) object owned by this view.
