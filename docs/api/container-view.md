# ContainerView

A `ContainerView` can be used to embed additional views hierarchy into a
[`BrowserWindow`](browser-window.md).

## Class: ContainerView

> Create and control views.

Process: [Main](../glossary.md#main-process)

### Example

```javascript
// In the main process.
const { ContainerView, BrowserWindow } = require('electron')

const win = new BrowserWindow({ width: 800, height: 600 })

const view = new ContainerView()
win.addContainerView(view)
view.setBounds({ x: 0, y: 0, width: 300, height: 300 })
```

### `new ContainerView()`

### Instance Methods

Objects created with `new ContainerView` have the following instance methods:

#### `view.setAutoResize(options)`

* `options` Object
  * `width` Boolean (optional) - If `true`, the view's width will grow and shrink together
    with the window. `false` by default.
  * `height` Boolean (optional) - If `true`, the view's height will grow and shrink
    together with the window. `false` by default.
  * `horizontal` Boolean (optional) - If `true`, the view's x position and width will grow
    and shrink proportionally with the window. `false` by default.
  * `vertical` Boolean (optional) - If `true`, the view's y position and height will grow
    and shrink proportionally with the window. `false` by default.

#### `view.setBounds(bounds)`

* `bounds` [Rectangle](structures/rectangle.md)

Resizes and moves the view to the supplied bounds relative to the parent (window or `containerView`).

#### `view.getBounds()`

Returns [`Rectangle`](structures/rectangle.md)

The `bounds` of this ContainerView instance as `Object`.

#### `view.setBackgroundColor(color)`

* `color` String - Color in `#aarrggbb` or `#argb` form. The alpha channel is
  optional.

#### `view.addBrowserView(browserView)`

* `browserView` [BrowserView](browser-view.md)

#### `view.removeBrowserView(browserView)`

* `browserView` [BrowserView](browser-view.md)

#### `view.setTopBrowserView(browserView)`

* `browserView` [BrowserView](browser-view.md)

Raises `browserView` above other `BrowserView`s and `Container`s attached
to `view`.
Throws an error if `browserView` is not attached to `view`.

#### `view.getBrowserViews()`

Returns `BrowserView[]` - an array of all BrowserViews that have been attached
with `addBrowserView`.

#### `view.addContainerView(containerView)`

* `containerView` [ContainerView](container-view.md)

#### `view.removeContainerView(containerView)`

* `containerView` [ContainerView](container-view.md)

#### `view.setTopContainerView(containerView)`

* `containerView` [ContainerView](container-view.md)

Raises `containerView` above other `ContainerView`s and `BrowserView`s attached
to `view`.
Throws an error if `containerView` is not attached to `view`.

#### `view.getContainerViews()`

Returns `ContainerView[]` - an array of all ContainerViews that have been attached
with `addContainerView`.
