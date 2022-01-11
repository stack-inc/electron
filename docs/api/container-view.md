# ContainerView

A `ContainerView` can be used to embed additional views hierarchy into a
[`BrowserWindow`](browser-window.md).
It extends [`BaseView`](base-view.md).

## Class: ContainerView extends `BaseView`

> Create and control views.

Process: [Main](../glossary.md#main-process)

### Example

```javascript
// In the main process.
const { ContainerView, BrowserWindow } = require('electron')

const win = new BrowserWindow({ width: 800, height: 600 })

const view = new ContainerView()
win.addChildView(view)
view.setBounds({ x: 0, y: 0, width: 300, height: 300 })
```

### `new ContainerView()`

### Instance Methods

Objects created with `new ContainerView` have the following instance methods:

#### `view.addBrowserView(browserView)`

* `browserView` [BrowserView](browser-view.md)

#### `view.removeBrowserView(browserView)`

* `browserView` [BrowserView](browser-view.md)

#### `view.setTopBrowserView(browserView)`

* `browserView` [BrowserView](browser-view.md)

Raises `browserView` above other `BrowserView`s and `BaseView`s attached
to `view`.
Throws an error if `browserView` is not attached to `view`.

#### `view.getBrowserViews()`

Returns `BrowserView[]` - an array of all BrowserViews that have been attached
with `addBrowserView`.

#### `view.addChildView(view)`

* `view` [BaseView](base-view.md)

#### `view.removeChildView(view)`

* `view` [BaseView](base-view.md)

#### `view.setTopChildView(view)`

* `view` [BaseView](base-view.md)

Raises `view` above other `BaseView`s and `BrowserView`s attached
to `view`.
Throws an error if `containerView` is not attached to `view`.

#### `view.getViews()`

Returns `BaseView[]` - an array of all BaseViews that have been attached
with `addChildView`.
