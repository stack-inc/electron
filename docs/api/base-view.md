# BaseView

A `BaseView` can be used to embed additional views hierarchy into a
[`BrowserWindow`](browser-window.md).

## Class: BaseView

> Create and control views.

Process: [Main](../glossary.md#main-process)

### Example

```javascript
// In the main process.
const { BaseView, BrowserWindow } = require('electron')

const win = new BrowserWindow({ width: 800, height: 600 })

const view = new BaseView()
win.addBaseView(view)
view.setBounds({ x: 0, y: 0, width: 300, height: 300 })
```

### `new BaseView()`

### Instance Methods

Objects created with `new BaseView` have the following instance methods:

#### `view.setBounds(bounds)`

* `bounds` [Rectangle](structures/rectangle.md)

Resizes and moves the view to the supplied bounds relative to the parent (window or `containerView`).

#### `view.getBounds()`

Returns [`Rectangle`](structures/rectangle.md)

The `bounds` of this BaseView instance as `Object`.

#### `view.setBackgroundColor(color)`

* `color` String - Color in `#aarrggbb` or `#argb` form. The alpha channel is
  optional.
