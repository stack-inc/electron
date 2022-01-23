import { BaseView } from 'electron/main';
import type { ContainerView as CVT } from 'electron/main';
const { ContainerView } = process._linkedBinding('electron_browser_container_view') as { ContainerView: typeof CVT };

Object.setPrototypeOf(ContainerView.prototype, BaseView.prototype);

module.exports = ContainerView;
