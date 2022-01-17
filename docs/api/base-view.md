# BaseView

A `BaseView` is a rectangle within the views View hierarchy. It is the base
class for [`ContainerView`](container-view.md), [`ScrollView`](scroll-view.md).

## Class: BaseView

> Base view for all different views.

Process: [Main](../glossary.md#main-process)

### `new BaseView()` _Experimental_

### Instance Methods

Objects created with `new BaseView` have the following instance methods:

#### `view.setBounds(bounds)` _Experimental_

* `bounds` [Rectangle](structures/rectangle.md)

Resizes and moves the view to the supplied bounds relative to the parent.

#### `view.getBounds()` _Experimental_

Returns [`Rectangle`](structures/rectangle.md)

The `bounds` of this `BaseView` instance as `Object`.

#### `view.offsetFromView(from)` _macOS_ _Experimental_

* `from` [BaseView](base-view.md)

Returns [`Point`](structures/point.md)

Converts a point from the coordinate system of a given `from` to that of the `view`.
both `from` and the `view` must belong to the same `BrowserWindow` object.

#### `view.offsetFromWindow()` _macOS_ _Experimental_

Returns [`Point`](structures/point.md)

Converts a point from window base coordinates to that of the `view`.

#### `view.setVisible(visible)` _Experimental_

* `visible` Boolean - Sets whether this `view` is visible.

#### `view.isVisible()` _Experimental_

Returns `Boolean` - Whether the `view` is visible.

#### `view.IsTreeVisible()` _Experimental_

Returns `Boolean` - Whether the `view` and its parent are visible.

#### `view.focus()` _Experimental_

Move the keyboard focus to the `view`.

#### `view.hasFocus()` _Experimental_

Returns `Boolean` - Whether this `view` currently has the focus.

#### `view.setFocusable(focusable)` _Experimental_

* `focusable` Boolean - Sets whether this `view` can be focused.

#### `view.isFocusable()` _Experimental_

Returns `Boolean` - Returns `true` if this `view` is focusable, enabled and drawn.

#### `view.setBackgroundColor(color)` _Experimental_

* `color` String - Color in `#aarrggbb` or `#argb` form. The alpha channel is
  optional.
