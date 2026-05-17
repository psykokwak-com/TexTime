
const char PAGE_style_css[] PROGMEM = R"=====(
:root {
  --primary-color: #2563eb;
  --primary-hover: #1d4ed8;
  --primary-active: #1e40af;
  --secondary-color: #64748b;
  --success-color: #10b981;
  --warning-color: #f59e0b;
  --error-color: #ef4444;
  --background: #ffffff;
  --surface: #f8fafc;
  --surface-hover: #f1f5f9;
  --text-primary: #1e293b;
  --text-secondary: #64748b;
  --text-light: #94a3b8;
  --border: #e2e8f0;
  --border-hover: #cbd5e1;
  --shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.1), 0 1px 2px 0 rgba(0, 0, 0, 0.06);
  --shadow-md: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06);
  --shadow-lg: 0 10px 15px -3px rgba(0, 0, 0, 0.1), 0 4px 6px -2px rgba(0, 0, 0, 0.05);
  --radius: 0.5rem;
  --radius-sm: 0.25rem;
  --radius-lg: 0.75rem;
}

[data-theme="dark"] {
  --primary-color: #3b82f6;
  --primary-hover: #2563eb;
  --primary-active: #1d4ed8;
  --secondary-color: #6b7280;
  --success-color: #059669;
  --warning-color: #d97706;
  --error-color: #dc2626;
  --background: #0f172a;
  --surface: #1e293b;
  --surface-hover: #334155;
  --text-primary: #f1f5f9;
  --text-secondary: #cbd5e1;
  --text-light: #94a3b8;
  --border: #334155;
  --border-hover: #475569;
  --shadow: 0 1px 3px 0 rgba(0, 0, 0, 0.3), 0 1px 2px 0 rgba(0, 0, 0, 0.2);
  --shadow-md: 0 4px 6px -1px rgba(0, 0, 0, 0.3), 0 2px 4px -1px rgba(0, 0, 0, 0.2);
  --shadow-lg: 0 10px 15px -3px rgba(0, 0, 0, 0.3), 0 4px 6px -2px rgba(0, 0, 0, 0.2);
}

* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
  line-height: 1.6;
  color: var(--text-primary);
  background-color: var(--background);
  font-size: 14px;
  min-height: 100vh;
  transition: background-color 0.3s ease, color 0.3s ease;
}

.app-layout {
  display: flex;
  min-height: 100vh;
}

.sidebar {
  width: 280px;
  background: var(--surface);
  border-right: 1px solid var(--border);
  display: flex;
  flex-direction: column;
  position: fixed;
  height: 100vh;
  overflow-y: auto;
  transform: translateX(-100%);
  transition: transform 0.3s ease;
  z-index: 1000;
}

.sidebar.open {
  transform: translateX(0);
}

.main-content {
  flex: 1;
  background: var(--background);
  min-height: 100vh;
}

.container {
  max-width: none;
  margin: 0;
  padding: 1.5rem;
  width: 100%;
}

.dashboard-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 2rem;
  padding-bottom: 1rem;
  border-bottom: 1px solid var(--border);
}

.header-left {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.header-right {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.menu-toggle {
  display: none;
  background: none;
  border: none;
  font-size: 1.5rem;
  color: var(--text-primary);
  cursor: pointer;
  padding: 0.5rem;
  border-radius: var(--radius-sm);
  transition: background-color 0.2s;
}

.menu-toggle:hover {
  background: var(--surface-hover);
}

.theme-toggle {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  padding: 0.5rem;
  cursor: pointer;
  color: var(--text-primary);
  font-size: 1.2rem;
  transition: all 0.2s;
}

.theme-toggle:hover {
  background: var(--surface-hover);
  border-color: var(--border-hover);
}

.dashboard-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(400px, 1fr));
  gap: 1.5rem;
  margin-bottom: 2rem;
}

.sidebar-header {
  padding: 1.5rem;
  border-bottom: 1px solid var(--border);
}

.sidebar-title {
  font-size: 1.25rem;
  font-weight: 600;
  color: var(--text-primary);
  margin: 0;
}

.sidebar-nav {
  flex: 1;
  padding: 1rem 0;
}

.nav-item {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  padding: 0.75rem 1.5rem;
  color: var(--text-secondary);
  text-decoration: none;
  transition: all 0.2s;
  border: none;
  background: none;
  width: 100%;
  text-align: left;
  cursor: pointer;
  font-size: 0.875rem;
}

.nav-item:hover {
  background: var(--surface-hover);
  color: var(--text-primary);
}

.nav-item.active {
  background: var(--primary-color);
  color: white;
}

.nav-item.active:hover {
  background: var(--primary-hover);
}

.nav-icon {
  font-size: 1.1rem;
  width: 1.5rem;
  text-align: center;
}

.overlay {
  display: none;
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  z-index: 999;
}

.overlay.active {
  display: block;
}

.header {
  display: flex;
  align-items: center;
  gap: 1rem;
  margin-bottom: 2rem;
  padding-bottom: 1rem;
  border-bottom: 1px solid var(--border);
}

.header-title {
  font-size: 1.5rem;
  font-weight: 600;
  color: var(--text-primary);
}

.back-btn {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  width: 2rem;
  height: 2rem;
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  text-decoration: none;
  color: var(--text-secondary);
  font-size: 1.2rem;
  transition: all 0.2s;
}

.back-btn:hover {
  background: var(--surface-hover);
  border-color: var(--border-hover);
  color: var(--text-primary);
}

.card {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  padding: 1.5rem;
  margin-bottom: 1.5rem;
  box-shadow: var(--shadow);
  transition: box-shadow 0.2s;
}

.card:hover {
  box-shadow: var(--shadow-md);
}

.card-title {
  font-size: 1.1rem;
  font-weight: 600;
  color: var(--text-primary);
  margin-bottom: 1rem;
}

.card.compact-status {
  padding: 1rem 1.5rem;
  margin-bottom: 1rem;
}

.card.compact-status .card-title {
  margin-bottom: 0.5rem;
  font-size: 1rem;
}

.card.compact-status .status-item {
  margin-bottom: 0;
}

.form-group {
  margin-bottom: 1.5rem;
}

.form-label {
  display: block;
  font-weight: 500;
  color: var(--text-primary);
  margin-bottom: 0.5rem;
  font-size: 0.875rem;
}

.form-control {
  width: 100%;
  padding: 0.75rem;
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  font-size: 0.875rem;
  transition: all 0.2s;
  background: var(--background);
  color: var(--text-primary);
}

.form-control:focus {
  outline: none;
  border-color: var(--primary-color);
  box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.1);
}

.form-control:disabled {
  background-color: var(--surface);
  color: var(--text-light);
  cursor: not-allowed;
}

.form-control-group {
  display: flex;
  gap: 0.5rem;
  align-items: center;
}

.form-control-group input {
  flex: 1;
}

.form-control-group span {
  color: var(--text-secondary);
  font-size: 0.875rem;
}

.checkbox-group {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.checkbox {
  width: 1rem;
  height: 1rem;
  accent-color: var(--primary-color);
}

.btn {
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
  padding: 0.75rem 1.5rem;
  border: 1px solid transparent;
  border-radius: var(--radius-sm);
  font-size: 0.875rem;
  font-weight: 500;
  text-decoration: none;
  text-align: center;
  cursor: pointer;
  transition: all 0.2s;
  line-height: 1;
  white-space: nowrap;
  user-select: none;
}

.btn-primary {
  background: var(--primary-color);
  color: white;
  border-color: var(--primary-color);
}

.btn-primary:hover {
  background: var(--primary-hover);
  border-color: var(--primary-hover);
  color: white;
}

.btn-primary:active {
  background: var(--primary-active);
  border-color: var(--primary-active);
}

.btn-secondary {
  background: var(--surface);
  color: var(--text-primary);
  border-color: var(--border);
}

.btn-secondary:hover {
  background: var(--surface-hover);
  border-color: var(--border-hover);
  color: var(--text-primary);
}

.btn-sm {
  padding: 0.5rem 1rem;
  font-size: 0.75rem;
}

.btn-lg {
  padding: 1rem 2rem;
  font-size: 1rem;
}

.btn-block {
  width: 100%;
}

.menu-grid {
  display: grid;
  grid-template-columns: 1fr;
  gap: 1rem;
  margin-bottom: 2rem;
}

@media (min-width: 480px) {
  .menu-grid {
    grid-template-columns: repeat(2, 1fr);
  }
}

.menu-item {
  display: flex;
  align-items: center;
  gap: 1rem;
  padding: 1.5rem;
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  text-decoration: none;
  color: var(--text-primary);
  transition: all 0.2s;
  box-shadow: var(--shadow);
}

.menu-item:hover {
  background: var(--surface-hover);
  border-color: var(--border-hover);
  box-shadow: var(--shadow-md);
  transform: translateY(-1px);
  color: var(--text-primary);
}

.menu-item-icon {
  width: 2.5rem;
  height: 2.5rem;
  background: var(--primary-color);
  border-radius: var(--radius-sm);
  display: flex;
  align-items: center;
  justify-content: center;
  color: white;
  font-size: 1.2rem;
  flex-shrink: 0;
}

.menu-item-content {
  flex: 1;
}

.menu-item-title {
  font-weight: 600;
  font-size: 1rem;
  margin-bottom: 0.25rem;
}

.menu-item-subtitle {
  color: var(--text-secondary);
  font-size: 0.875rem;
}

.range-group {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.range-input {
  flex: 1;
  margin: 0;
}

.range-value {
  min-width: 3rem;
  padding: 0.5rem;
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  font-size: 0.875rem;
  text-align: center;
  color: var(--text-primary);
  font-weight: 500;
}

.color-picker {
  display: flex;
  align-items: center;
  gap: 0.75rem;
}

.color-picker-wrapper {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  flex-wrap: wrap;
}

.color-input {
  width: 3rem;
  height: 2.5rem;
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  cursor: pointer;
  padding: 0;
  background: none;
}

.color-text {
  width: 8rem;
  font-family: monospace;
}

.color-preview {
  width: 2.5rem;
  height: 2.5rem;
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  cursor: pointer;
  background-color: #ffffff;
}

/* Simple Color Picker */
.color-picker-container {
  display: flex;
  gap: 0.75rem;
  align-items: center;
}

.color-picker-input {
  width: 3rem;
  height: 2.5rem;
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  cursor: pointer;
  padding: 0;
  background: none;
}

.color-picker-input:focus {
  outline: none;
  border-color: var(--primary-color);
  box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.1);
}

.cpicker-swatch {
  width:3rem; height:2.5rem;
  border:2px solid var(--border);
  border-radius:var(--radius-sm);
  cursor:pointer; flex-shrink:0;
  background:#ff0000;
  transition:border-color 0.15s;
}
.cpicker-swatch:hover { border-color:var(--primary-color); }
.cpicker-panel { margin-top:0.5rem; user-select:none; -webkit-user-select:none; }
.cpicker-sv {
  position:relative; width:100%; height:160px;
  border-radius:var(--radius-sm);
  overflow:hidden; cursor:crosshair; touch-action:none;
}
.cpicker-sv-white { position:absolute; inset:0; background:linear-gradient(to right,#fff,transparent); }
.cpicker-sv-black { position:absolute; inset:0; background:linear-gradient(to bottom,transparent,#000); }
.cpicker-sv-thumb {
  position:absolute; width:14px; height:14px;
  border:2px solid #fff; border-radius:50%;
  box-shadow:0 0 3px rgba(0,0,0,.6);
  transform:translate(-50%,-50%); pointer-events:none;
}
.cpicker-hue {
  width:100%; margin-top:0.5rem; height:1.5rem;
  -webkit-appearance:none; appearance:none;
  background:linear-gradient(to right,#f00,#ff0,#0f0,#0ff,#00f,#f0f,#f00);
  border-radius:var(--radius-sm); cursor:pointer; border:none; outline:none;
}
.cpicker-hue::-webkit-slider-thumb {
  -webkit-appearance:none; width:18px; height:1.5rem;
  background:rgba(255,255,255,.9); border:2px solid #888;
  border-radius:4px; cursor:pointer;
}
.cpicker-hue::-moz-range-thumb {
  width:18px; height:1.5rem;
  background:rgba(255,255,255,.9); border:2px solid #888;
  border-radius:4px; cursor:pointer;
}

.color-text-input {
  flex: 1;
  padding: 0.75rem;
  border: 1px solid var(--border);
  border-radius: var(--radius-sm);
  font-size: 0.875rem;
  font-family: monospace;
  transition: all 0.2s;
  background: var(--background);
  color: var(--text-primary);
}

.color-text-input:focus {
  outline: none;
  border-color: var(--primary-color);
  box-shadow: 0 0 0 3px rgba(37, 99, 235, 0.1);
}

@media (max-width: 480px) {
  .color-picker-container {
    flex-direction: column;
    align-items: stretch;
  }
  
  .color-picker-input {
    width: 100%;
  }
}

.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  gap: 1rem;
  margin-bottom: 2rem;
}

.status-item {
  padding: 1rem;
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  text-align: center;
}

.status-label {
  font-size: 0.875rem;
  color: var(--text-secondary);
  margin-bottom: 0.5rem;
}

.status-value {
  font-size: 1.125rem;
  font-weight: 600;
  color: var(--text-primary);
}

.networks-list {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  overflow: hidden;
  max-height: 400px;
  overflow-y: auto;
}

.network-item {
  display: flex;
  align-items: center;
  gap: 1rem;
  padding: 1rem 1.25rem;
  border-bottom: 1px solid var(--border);
  cursor: pointer;
  transition: all 0.2s;
  position: relative;
}

.network-item:last-child {
  border-bottom: none;
}

.network-item:hover {
  background: var(--surface-hover);
  transform: translateX(2px);
}

.network-item.connected {
  background: rgba(16, 185, 129, 0.1);
  border-left: 3px solid var(--success-color);
}

.network-info {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: 0.25rem;
}

.network-name {
  font-weight: 500;
  color: var(--text-primary);
  font-size: 0.875rem;
}

.network-details {
  display: flex;
  align-items: center;
  gap: 1rem;
  font-size: 0.75rem;
  color: var(--text-secondary);
}

.network-signal {
  display: flex;
  align-items: center;
  gap: 0.25rem;
}

.signal-bars {
  display: flex;
  align-items: end;
  gap: 1px;
  height: 12px;
}

.signal-bar {
  width: 2px;
  background: var(--text-light);
  border-radius: 1px;
}

.signal-bar.active {
  background: var(--success-color);
}

.signal-bar:nth-child(1) { height: 20%; }
.signal-bar:nth-child(2) { height: 40%; }
.signal-bar:nth-child(3) { height: 60%; }
.signal-bar:nth-child(4) { height: 80%; }
.signal-bar:nth-child(5) { height: 100%; }

.network-security {
  display: flex;
  align-items: center;
  gap: 0.25rem;
}

.security-icon {
  font-size: 0.875rem;
}

.network-actions {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.connect-btn {
  padding: 0.25rem 0.75rem;
  background: var(--primary-color);
  color: white;
  border: none;
  border-radius: var(--radius-sm);
  font-size: 0.75rem;
  cursor: pointer;
  transition: background-color 0.2s;
}

.connect-btn:hover {
  background: var(--primary-hover);
}

.connected-badge {
  padding: 0.125rem 0.5rem;
  background: var(--success-color);
  color: white;
  border-radius: var(--radius-sm);
  font-size: 0.625rem;
  font-weight: 500;
}

.loading {
  text-align: center;
  padding: 2rem;
  color: var(--text-secondary);
}

.footer {
  text-align: center;
  padding: 2rem 1rem;
  margin-top: auto;
  border-top: 1px solid var(--border);
  color: var(--text-secondary);
  font-size: 0.875rem;
}

.footer a {
  color: var(--primary-color);
  text-decoration: none;
}

.footer a:hover {
  text-decoration: underline;
}

.alert {
  padding: 1rem;
  border-radius: var(--radius);
  margin-bottom: 1rem;
  border: 1px solid;
}

.alert-success {
  background: #f0fdf4;
  border-color: #bbf7d0;
  color: #166534;
}

.alert-warning {
  background: #fffbeb;
  border-color: #fed7aa;
  color: #92400e;
}

.alert-error {
  background: #fef2f2;
  border-color: #fecaca;
  color: #991b1b;
}

.content-section {
  display: none;
  animation: slideIn 0.3s ease-out;
}

.content-section.active {
  display: block;
}

@keyframes slideIn {
  from {
    opacity: 0;
    transform: translateX(20px);
  }
  to {
    opacity: 1;
    transform: translateX(0);
  }
}

.loading-spinner {
  display: inline-block;
  width: 1rem;
  height: 1rem;
  border: 2px solid var(--border);
  border-radius: 50%;
  border-top-color: var(--primary-color);
  animation: spin 1s ease-in-out infinite;
}

@keyframes spin {
  to {
    transform: rotate(360deg);
  }
}

.form-actions {
  display: flex;
  gap: 1rem;
  margin-top: 1.5rem;
}

.form-actions .btn {
  flex: 1;
}

.status-indicator {
  display: inline-flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.25rem 0.75rem;
  border-radius: var(--radius-sm);
  font-size: 0.875rem;
  font-weight: 500;
}

.status-indicator.connected {
  background: var(--success-color);
  color: white;
}

.status-indicator.disconnected {
  background: var(--error-color);
  color: white;
}

.status-indicator.connecting {
  background: var(--warning-color);
  color: white;
}

.topic-list {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  padding: 1rem;
  font-family: 'Courier New', monospace;
  font-size: 0.875rem;
  line-height: 1.5;
  overflow-x: auto;
}

.topic-item {
  margin-bottom: 0.5rem;
  padding: 0.5rem;
  background: var(--background);
  border-radius: var(--radius-sm);
  border-left: 3px solid var(--primary-color);
}

.topic-name {
  font-weight: 600;
  color: var(--primary-color);
}

.topic-description {
  color: var(--text-secondary);
  margin-top: 0.25rem;
}

/* Desktop Layout */
@media (min-width: 1024px) {
  .sidebar {
    position: static;
    transform: translateX(0);
    width: 280px;
  }
  
  .main-content {
    margin-left: 0;
  }
  
  .menu-toggle {
    display: none;
  }
  
  .dashboard-grid {
    grid-template-columns: repeat(auto-fit, minmax(500px, 1fr));
  }
  
  .status-grid {
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  }
}

/* Tablet Layout */
@media (max-width: 1023px) and (min-width: 769px) {
  .menu-toggle {
    display: block;
  }
  
  .dashboard-grid {
    grid-template-columns: repeat(auto-fit, minmax(400px, 1fr));
  }
  
  .status-grid {
    grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
  }
}

/* Mobile Layout */
@media (max-width: 768px) {
  .menu-toggle {
    display: block;
  }
  
  .dashboard-header {
    justify-content: space-between;
    flex-wrap: wrap;
    gap: 1rem;
  }
  
  .header-left {
    order: 2;
    flex: 1;
  }
  
  .header-right {
    order: 3;
  }
  
  .menu-toggle {
    order: 1;
  }
  
  .dashboard-grid {
    grid-template-columns: 1fr;
    gap: 1rem;
  }
  
  .status-grid {
    grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
  }
  
  .container {
    padding: 1rem;
  }
  
  .card {
    padding: 1rem;
  }
  
  .form-control-group {
    flex-direction: column;
    align-items: stretch;
    gap: 0.5rem;
  }
  
  .range-group {
    flex-direction: column;
    align-items: stretch;
    gap: 0.5rem;
  }
  
  .range-value {
    align-self: center;
    min-width: 2rem;
  }
  
  .form-actions {
    flex-direction: column;
  }
  
  .network-item {
    padding: 0.75rem 1rem;
  }
  
  .network-details {
    flex-wrap: wrap;
    gap: 0.5rem;
  }
}

@media (max-width: 480px) {
  .container {
    padding: 0.75rem;
  }
  
  .dashboard-header {
    margin-bottom: 1rem;
    padding-bottom: 0.75rem;
  }
  
  .header-title {
    font-size: 1.25rem;
  }
  
  .sidebar {
    width: 100%;
  }
  
  .network-info {
    gap: 0.125rem;
  }
  
  .network-details {
    font-size: 0.675rem;
  }
  
  .signal-bars {
    height: 10px;
  }
}

/* Scheduler editor card — compact overrides */
.sched-editor { padding: 0.65rem !important; }
.sched-editor .card-title { font-size: 0.95rem; margin-bottom: 0.35rem; }
.sched-editor .form-group { margin-bottom: 0.3rem; }
.sched-editor .form-label { font-size: 0.72rem; margin-bottom: 0.1rem; }
.sched-editor .form-control { padding: 0.22rem 0.45rem; font-size: 0.78rem; }
.sched-editor .range-group { gap: 0.3rem; }
.sched-editor .range-input { height: 3px; }
.sched-editor .range-value { font-size: 0.75rem; min-width: 1.8rem; }
.sched-editor .sched-rule-item { padding: 0.25rem 0.5rem; margin-bottom: 0.2rem; }
.sched-editor .sched-rule-num { min-width: 1rem; height: 1rem; font-size: 0.65rem; }
.sched-editor .sched-rule-led { width: 0.85rem; height: 0.85rem; }
.sched-editor .btn { padding: 0.28rem 0.6rem; font-size: 0.8rem; }

/* Scheduler grid */
.sched-grid-wrap { overflow-x: auto; margin-top: 0.5rem; user-select: none; -webkit-user-select: none; }
.sched-table { border-collapse: collapse; width: 100%; font-size: 0.72rem; }
.sched-table th { padding: 0.25rem 0.4rem; background: var(--surface); border: 1px solid var(--border); text-align: center; font-weight: 600; }
.sched-hour { padding: 0.25rem 0.4rem; background: var(--surface); border: 1px solid var(--border); font-weight: 500; white-space: nowrap; font-size: 0.7rem; }
.sched-cell { border: 1px solid var(--border); min-width: 2.2rem; overflow: hidden; }
.sched-half { text-align: center; cursor: pointer; height: 0.9rem; line-height: 0.9rem; font-size: 0.6rem; overflow: hidden; transition: opacity 0.15s; }
.sched-half:hover { opacity: 0.75; outline: 2px solid var(--primary-color); outline-offset: -1px; }
.sched-half + .sched-half { border-top: 1px solid rgba(128,128,128,0.25); }
.sched-half.sched-cur { outline: 2px solid #fff; outline-offset: -2px; }
/* Scheduler rules list */
.sched-rule-item { display:flex; align-items:center; gap:0.5rem; padding:0.4rem 0.6rem; border:1px solid var(--border); border-radius:var(--radius-sm); margin-bottom:0.35rem; cursor:pointer; transition:background 0.15s; }
.sched-rule-item:hover { background:var(--surface-hover); }
.sched-rule-active { border-color:var(--primary-color) !important; background:rgba(37,99,235,0.07); }
.sched-rule-num { min-width:1.2rem; height:1.2rem; border-radius:0.2rem; background:var(--primary-color); color:#fff; font-size:0.7rem; font-weight:700; display:flex; align-items:center; justify-content:center; flex-shrink:0; padding:0 0.2rem; }
.sched-rule-led { width:1rem; height:1rem; border-radius:2px; border:1px solid var(--border); flex-shrink:0; }
.sched-rule-del { background:none; border:none; cursor:pointer; color:var(--text-secondary); font-size:0.75rem; padding:0.1rem 0.3rem; border-radius:2px; line-height:1; }
.sched-rule-del:hover { background:var(--error-color); color:#fff; }
)=====";

