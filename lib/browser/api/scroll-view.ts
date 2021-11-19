import { ContainerView } from 'electron/main';
import type { ScrollView as SVT } from 'electron/main';
const { ScrollView } = process._linkedBinding('electron_browser_scroll_view') as { ScrollView: typeof SVT };

Object.setPrototypeOf(ScrollView.prototype, ContainerView.prototype);

module.exports = ScrollView;
