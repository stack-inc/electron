# ScrollView

Show a part of view with scrollbar. 
The `ScrollView` can show an arbitrary content view inside it. It is used to make
any View scrollable. When the content is larger than the `ScrollView`,
scrollbars will be optionally showed. When the content view is smaller
then the `ScrollView`, the content view will be resized to the size of the
`ScrollView`.
The scrollview supports keyboard UI and mousewheel.
It extends [`BaseView`](base-view.md).

## Class: ScrollView  extends `BaseView`

> Create and control views.

Process: [Main](../glossary.md#main-process)

### Example

```javascript
// In the main process.
const { ScrollView, BrowserWindow } = require('electron')

const win = new BrowserWindow({ width: 800, height: 600 })

const view = new ScrollView()
win.addScrollView(view)
view.setBounds({ x: 0, y: 0, width: 300, height: 300 })
```

### `new ScrollView()` _Experimental_

### Instance Methods

Objects created with `new ScrollView` have the following instance methods:

#### `view.setContentView(contents)` _Experimental_

* `contents` [BaseView](base-view.md)

Set the contents. The contents is the view that needs to scroll.

  #### `view.getContentView()` _Experimental_

Returns [`BaseView`](base-view.md)

The contents of the `view`.

#### `view.setContentSize(size)` _Experimental_

* `size` [Size](structures/size.md)

Set the size of the contents.

#### `view.getContentSize()` _Experimental_

Returns [`Size`](structures/size.md)

The `size` of the contents.

#### `view.getMinHeight()` _Experimental_

Returns `Integer` - The min height for the bounded scroll view.

This is negative value if the view is not bounded.

#### `view.getMaxHeight()` _Experimental_

Returns `Integer` - The max height for the bounded scroll view.

This is negative value if the view is not bounded.

#### `view.clipHeightTo(minHeight, maxHeight)` _Experimental_

* `minHeight` Integer - The min height for the bounded scroll view.
* `maxHeight` Integer - The max height for the bounded scroll view.

Turns this scroll view into a bounded scroll view, with a fixed height.
By default, a ScrollView will stretch to fill its outer container.

#### `view.getVisibleRect()` _Experimental_

Returns [`Rectangle`](structures/rectangle.md) - The visible region of the content View.

#### `view.setHorizontalScrollBarMode(mode)` _Experimental_

* `mode` String - Can be one of the following values: `disabled`, `hidden-but-enabled`, `enabled`. Default is `enabled`.

Controls how the horizontal scroll bar appears and functions.
* `disable` - The scrollbar is hidden, and the pane will not respond to e.g. mousewheel events even if the contents are larger than the viewport.
* `hidden-but-enabled` - The scrollbar is hidden whether or not the contents are larger than the viewport, but the pane will respond to scroll events.
*`enabled` - The scrollbar will be visible if the contents are larger than the viewport and the pane will respond to scroll events.

#### `view.getHorizontalScrollBarMode()` _Experimental_

Returns `String` - horizontal scrollbar mode.

#### `view.setVerticalScrollBarMode(mode)` _Experimental_

* `mode` String - Can be one of the following values: `disabled`, `hidden-but-enabled`, `enabled`. Default is `enabled`.

Controls how the vertical scroll bar appears and functions.
* `disable` - The scrollbar is hidden, and the pane will not respond to e.g. mousewheel events even if the contents are larger than the viewport.
* `hidden-but-enabled` - The scrollbar is hidden whether or not the contents are larger than the viewport, but the pane will respond to scroll events.
*`enabled` - The scrollbar will be visible if the contents are larger than the viewport and the pane will respond to scroll events.

#### `view.getVerticalScrollBarMode()` _Experimental_

Returns `String` - vertical scrollbar mode.

#### `view.setAllowKeyboardScrolling(allow)` _Experimental_

* `allow` Boolean - Sets whether the left/right/up/down arrow keys attempt to scroll the view.

#### `view.getAllowKeyboardScrolling()` _Experimental_

Returns `Boolean` - Gets whether the keyboard arrow keys attempt to scroll the view. Default is `true`.

#### `view.SetDrawOverflowIndicator(indicator)` _Experimental_

* `indicator` Boolean - Sets whether to draw a white separator on the four sides of the scroll view when it overflows.

#### `view.GetDrawOverflowIndicator()` _Experimental_

Returns `Boolean` - Gets whether to draw a white separator on the four sides of the scroll view when it overflows. Default is `true`.
