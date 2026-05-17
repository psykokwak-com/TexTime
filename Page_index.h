//
//  HTML PAGE
//

const char PAGE_index[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en" data-theme="light">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>TexTime - Word Clock Dashboard</title>
  <script src="/microajax.js"></script>
</head>
<body>
  <div class="app-layout">
    <!-- Sidebar Navigation -->
    <nav class="sidebar" id="sidebar">
      <div class="sidebar-header">
        <h1 class="sidebar-title">TexTime</h1>
      </div>
      <div class="sidebar-nav">
        <button class="nav-item active" data-section="info">
          <span class="nav-icon">ℹ</span>
          <span>System Information</span>
        </button>
        <button class="nav-item" data-section="general">
          <span class="nav-icon">⚙</span>
          <span>General Settings</span>
        </button>
        <button class="nav-item" data-section="network">
          <span class="nav-icon">📶</span>
          <span>Network Configuration</span>
        </button>
        <button class="nav-item" data-section="ntp">
          <span class="nav-icon">🕐</span>
          <span>Time Configuration</span>
        </button>
        <button class="nav-item" data-section="mqtt">
          <span class="nav-icon">📡</span>
          <span>MQTT Configuration</span>
        </button>
        <button class="nav-item" data-section="update">
          <span class="nav-icon">🔄</span>
          <span>Firmware Update</span>
        </button>
        <button class="nav-item" data-section="scheduler">
          <span class="nav-icon">&#x23F0;</span>
          <span>Scheduler</span>
        </button>
        <a class="nav-item" href="/tetris.html" style="text-decoration:none;">
          <span class="nav-icon">🎮</span>
          <span>Tetris</span>
        </a>
        <a class="nav-item" href="/snake.html" style="text-decoration:none;">
          <span class="nav-icon">🐍</span>
          <span>Snake</span>
        </a>
      </div>
    </nav>

    <!-- Main Content -->
    <main class="main-content">
      <div class="container">
        <!-- Dashboard Header -->
        <header class="dashboard-header">
          <div class="header-left">
            <button class="menu-toggle" id="menuToggle">☰</button>
            <h1 class="header-title" id="sectionTitle">System Information</h1>
          </div>
          <div class="header-right">
            <button class="theme-toggle" id="themeToggle" title="Toggle dark mode">🌙</button>
          </div>
        </header>

        <!-- System Information Section -->
        <section id="info-section" class="content-section active">
          <div class="dashboard-grid">
            <div class="card">
              <div class="card-title">System Status</div>
              <div class="status-grid">
                <div class="status-item">
                  <div class="status-label">Current Date</div>
                  <div class="status-value" id="x_date">Loading...</div>
                </div>
                <div class="status-item">
                  <div class="status-label">Uptime</div>
                  <div class="status-value" id="x_boot">Loading...</div>
                </div>
                <div class="status-item">
                  <div class="status-label">Firmware Build</div>
                  <div class="status-value" id="x_version">Loading...</div>
                </div>
              </div>
            </div>

            <div class="card">
              <div class="card-title">Network Information</div>
              <div class="status-grid">
                <div class="status-item">
                  <div class="status-label">WiFi Network</div>
                  <div class="status-value" id="x_ssid">Loading...</div>
                </div>
                <div class="status-item">
                  <div class="status-label">Signal Strength</div>
                  <div class="status-value"><span id="x_rssi">-</span>%</div>
                </div>
                <div class="status-item">
                  <div class="status-label">IP Address</div>
                  <div class="status-value" id="x_ip">Loading...</div>
                </div>
                <div class="status-item">
                  <div class="status-label">MAC Address</div>
                  <div class="status-value" id="x_mac">Loading...</div>
                </div>
              </div>
            </div>

            <div class="card">
              <div class="card-title">Sensor Readings</div>
              <div class="status-grid">
                <div class="status-item">
                  <div class="status-label">Ambient Light</div>
                  <div class="status-value"><span id="x_als">-</span> lux</div>
                </div>
                <div class="status-item">
                  <div class="status-label">Display Brightness</div>
                  <div class="status-value" id="x_brightness">-</div>
                </div>
                <div class="status-item">
                  <div class="status-label">Temperature</div>
                  <div class="status-value"><span id="x_temp">-</span> °C</div>
                </div>
              </div>
            </div>
          </div>
        </section>

        <!-- General Settings Section -->
        <section id="general-section" class="content-section">
          <div class="dashboard-grid">
            <div class="card">
              <div class="card-title">Brightness Control</div>
              <form id="general-form" onsubmit="return saveGeneralSettings(event)">

            <div class="form-group">
              <div class="checkbox-group">
                <input type="checkbox" id="brightnessauto" name="brightnessauto" class="checkbox" onchange="updatebrightnessauto()">
                <label for="brightnessauto" class="form-label">Automatic brightness</label>
              </div>
            </div>

            <div class="form-group">
              <label for="brightnesssensibility" class="form-label">Brightness sensitivity</label>
              <select id="brightnesssensibility" name="brightnesssensibility" class="form-control" onchange="updatebrightnesssensibility()">
                <option value="10">Very high</option>
                <option value="20">High</option>
                <option value="30">Normal</option>
                <option value="40">Low</option>
                <option value="50">Very low</option>
              </select>
            </div>

            <div class="form-group">
              <label for="brightness" class="form-label">Manual brightness</label>
              <div class="range-group">
                <input type="range" id="brightness" name="brightness" class="range-input" max="255" min="0" step="1" oninput="updatebrightness()">
                <div class="range-value" id="brightnesst">--</div>
              </div>
            </div>

            <div class="form-group">
              <label for="brightnessday" class="form-label">Minimum day brightness</label>
              <div class="range-group">
                <input type="range" id="brightnessday" name="brightnessday" class="range-input" max="255" min="0" step="1" oninput="updatebrightnessday()">
                <div class="range-value" id="brightnessdayt">--</div>
              </div>
            </div>

            <div class="form-group">
              <label for="brightnessnight" class="form-label">Minimum night brightness</label>
              <div class="range-group">
                <input type="range" id="brightnessnight" name="brightnessnight" class="range-input" max="255" min="0" step="1" oninput="updatebrightnessnight()">
                <div class="range-value" id="brightnessnightt">--</div>
              </div>
            </div>
          </div>

            <div class="card">
              <div class="card-title">Display Configuration</div>

              <div class="form-group">
                <label for="ledconfig" class="form-label">Clock type</label>
                <select id="ledconfig" name="ledconfig" class="form-control">
                </select>
              </div>

            <div class="form-group">
              <label class="form-label">Color</label>
              <div style="display:flex;gap:0.75rem;align-items:center">
                <div id="colorSwatch" class="cpicker-swatch" title="Pick color" style="width:3rem;height:2.5rem;background:#ff0000;border:2px solid #ccc;border-radius:4px;cursor:pointer;flex-shrink:0"></div>
                <input type="text" id="colorText" class="color-text-input" placeholder="#FF0000" maxlength="7">
                <input type="hidden" id="color" name="color">
              </div>
              <div id="cpickerPanel" class="cpicker-panel" style="display:none">
                <div id="svSquare" class="cpicker-sv">
                  <div class="cpicker-sv-white"></div>
                  <div class="cpicker-sv-black"></div>
                  <div id="svThumb" class="cpicker-sv-thumb"></div>
                </div>
                <input type="range" id="hueSlider" class="cpicker-hue" min="0" max="359" value="0">
              </div>
            </div>

            <div class="form-group">
              <label for="colorrandom" class="form-label">Color randomization</label>
              <select id="colorrandom" name="colorrandom" class="form-control" onchange="updatecolorrandom()">
                <option value="0">No Random</option>
                <option value="1">Random all</option>
                <option value="2">Random letter</option>
                <option value="3">Random word</option>
              </select>
            </div>

            <div class="form-group">
              <label for="lang" class="form-label">Language</label>
              <select id="lang" name="lang" class="form-control" onchange="updatelang()">
              </select>
            </div>

            <div class="form-group">
              <label for="mode" class="form-label">Display mode</label>
              <select id="mode" name="mode" class="form-control" onchange="updatemode()">
              </select>
            </div>

                <div class="form-group">
                  <label for="animation" class="form-label">Animation</label>
                  <select id="animation" name="animation" class="form-control" onchange="updateanimation()">
                  </select>
                </div>

                <div class="form-group">
                  <label for="animspeed" class="form-label">Animation speed (1=slow, 20=fast)</label>
                  <div class="range-group">
                    <input type="range" id="animspeed" name="animspeed" class="range-input" min="1" max="20" step="1" oninput="updateanimspeed()">
                    <div class="range-value" id="animspeedt">--</div>
                  </div>
                </div>

                <div class="form-group">
                  <label for="animbrightmin" class="form-label">Animation min brightness (%)</label>
                  <div class="range-group">
                    <input type="range" id="animbrightmin" name="animbrightmin" class="range-input" min="0" max="100" step="1" oninput="updateanimbrightmin()">
                    <div class="range-value" id="animbrightmint">--</div>
                  </div>
                </div>

                <div class="form-group">
                  <label for="animbrightmax" class="form-label">Animation max brightness (%)</label>
                  <div class="range-group">
                    <input type="range" id="animbrightmax" name="animbrightmax" class="range-input" min="0" max="100" step="1" oninput="updateanimbrightmax()">
                    <div class="range-value" id="animbrightmaxt">--</div>
                  </div>
                </div>

                <button type="submit" class="btn btn-primary btn-block">Save Configuration</button>
              </form>
            </div>
          </div>
        </section>

        <!-- Network Configuration Section -->
        <section id="network-section" class="content-section">
          <div class="dashboard-grid">
            <div class="card compact-status">
              <div class="card-title">Connection Status</div>
              <div class="status-item">
                <div class="status-label">Current Status</div>
                <div class="status-value" id="connectionstatedisplay">Checking...</div>
                <div id="connectionstate" style="display: none;">Checking...</div>
              </div>
            </div>

            <div class="card">
              <div class="card-title">WiFi Settings</div>
              <form id="network-form" onsubmit="return saveNetworkSettings(event)">
            <div class="form-group">
              <label for="ssid" class="form-label">Network name (SSID)</label>
              <input type="text" id="ssid" name="ssid" class="form-control" placeholder="Enter network name">
            </div>

            <div class="form-group">
              <label for="password" class="form-label">Password</label>
              <input type="password" id="password" name="password" class="form-control" placeholder="Enter network password">
            </div>

            <div class="form-group">
              <label for="devicename" class="form-label">Device name</label>
              <input type="text" id="devicename" name="devicename" class="form-control" placeholder="TexTime">
            </div>

            <div class="form-group">
              <div class="checkbox-group">
                <input type="checkbox" id="dhcp" name="dhcp" class="checkbox" onchange="validateNetworkSettings()">
                <label for="dhcp" class="form-label">Use DHCP (automatic IP configuration)</label>
              </div>
            </div>

            <div id="static-config">
              <div class="form-group">
                <label class="form-label">IP Address</label>
                <input type="text" id="ipaddress" name="ipaddress" class="form-control" placeholder="192.168.1.100" pattern="^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$">
              </div>

              <div class="form-group">
                <label class="form-label">Subnet Mask</label>
                <input type="text" id="netmask" name="netmask" class="form-control" placeholder="255.255.255.0" pattern="^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$">
              </div>

              <div class="form-group">
                <label class="form-label">Gateway</label>
                <input type="text" id="gateway" name="gateway" class="form-control" placeholder="192.168.1.1" pattern="^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$">
              </div>

              <div class="form-group">
                <label class="form-label">DNS Server</label>
                <input type="text" id="dnsserver" name="dnsserver" class="form-control" placeholder="8.8.8.8" pattern="^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$">
              </div>
            </div>

                <button type="submit" class="btn btn-primary btn-block">Save & Restart</button>
              </form>
            </div>

            <div class="card">
              <div class="card-title">Available Networks</div>
              <div id="networks" class="networks-list">
                <div class="loading">Scanning for networks...</div>
              </div>
              <button type="button" onclick="refreshNetworks()" class="btn btn-secondary btn-block">Refresh Networks</button>
            </div>
          </div>
        </section>

        <!-- NTP Configuration Section -->
        <section id="ntp-section" class="content-section">
          <div class="dashboard-grid">
            <div class="card">
              <div class="card-title">Time Synchronization Settings</div>
              <form id="ntp-form" onsubmit="return saveNtpSettings(event)">
            <div class="form-group">
              <label for="ntpserver" class="form-label">NTP Server</label>
              <input type="text" id="ntpserver" name="ntpserver" class="form-control" maxlength="172" placeholder="pool.ntp.org">
            </div>

            <div class="form-group">
              <label for="update" class="form-label">Update interval</label>
              <div class="form-control-group">
                <input type="number" id="update" name="update" class="form-control" maxlength="6" placeholder="3600" min="0">
                <span>seconds (0 = disabled)</span>
              </div>
            </div>

            <div class="form-group">
              <label for="tz" class="form-label">Timezone</label>
              <select id="tz" name="tz" class="form-control">
                <option value="-120">(GMT-12:00) Baker Island</option>
                <option value="-110">(GMT-11:00) Samoa</option>
                <option value="-100">(GMT-10:00) Hawaii</option>
                <option value="-90">(GMT-09:00) Alaska</option>
                <option value="-80">(GMT-08:00) Pacific Time</option>
                <option value="-70">(GMT-07:00) Mountain Time</option>
                <option value="-60">(GMT-06:00) Central Time</option>
                <option value="-50">(GMT-05:00) Eastern Time</option>
                <option value="-40">(GMT-04:00) Atlantic Time</option>
                <option value="-35">(GMT-03:30) Newfoundland</option>
                <option value="-30">(GMT-03:00) Argentina</option>
                <option value="-20">(GMT-02:00) Mid-Atlantic</option>
                <option value="-10">(GMT-01:00) Azores</option>
                <option value="0">(GMT+00:00) GMT/UTC</option>
                <option value="10">(GMT+01:00) Central Europe</option>
                <option value="20">(GMT+02:00) Eastern Europe</option>
                <option value="30">(GMT+03:00) Russia</option>
                <option value="35">(GMT+03:30) Iran</option>
                <option value="40">(GMT+04:00) Gulf</option>
                <option value="45">(GMT+04:30) Afghanistan</option>
                <option value="50">(GMT+05:00) Pakistan</option>
                <option value="55">(GMT+05:30) India</option>
                <option value="57">(GMT+05:45) Nepal</option>
                <option value="60">(GMT+06:00) Bangladesh</option>
                <option value="65">(GMT+06:30) Myanmar</option>
                <option value="70">(GMT+07:00) Thailand</option>
                <option value="80">(GMT+08:00) China</option>
                <option value="90">(GMT+09:00) Japan</option>
                <option value="95">(GMT+09:30) Australia Central</option>
                <option value="100">(GMT+10:00) Australia Eastern</option>
                <option value="110">(GMT+11:00) Solomon Islands</option>
                <option value="120">(GMT+12:00) New Zealand</option>
                <option value="130">(GMT+13:00) Tonga</option>
              </select>
            </div>

            <div class="form-group">
              <div class="checkbox-group">
                <input type="checkbox" id="dst" name="dst" class="checkbox">
                <label for="dst" class="form-label">Enable daylight saving time</label>
              </div>
            </div>

                <button type="submit" class="btn btn-primary btn-block">Save Configuration</button>
              </form>
            </div>
          </div>
        </section>

        <!-- MQTT Configuration Section -->
        <section id="mqtt-section" class="content-section">
          <div class="dashboard-grid">
            <div class="card compact-status">
              <div class="card-title">Connection Status</div>
              <div class="status-item">
                <div class="status-label">MQTT Connection</div>
                <div class="status-value" id="mqttconnectionstate">Checking...</div>
              </div>
            </div>

            <div class="card">
              <div class="card-title">MQTT Broker Settings</div>
              <form id="mqtt-form" onsubmit="return saveMqttSettings(event)">
            <div class="form-group">
              <label for="host" class="form-label">Broker host</label>
              <input type="text" id="host" name="host" class="form-control" placeholder="mqtt.broker.com">
            </div>

            <div class="form-group">
              <label for="port" class="form-label">Broker port</label>
              <input type="number" id="port" name="port" class="form-control" placeholder="1883" min="1" max="65535">
            </div>

            <div class="form-group">
              <label for="login" class="form-label">Username (optional)</label>
              <input type="text" id="login" name="login" class="form-control" placeholder="Username">
            </div>

            <div class="form-group">
              <label for="mqttpassword" class="form-label">Password (optional)</label>
              <input type="password" id="mqttpassword" name="password" class="form-control" placeholder="Password">
            </div>

            <div class="form-group">
              <label for="interval" class="form-label">Publish interval</label>
              <div class="form-control-group">
                <input type="number" id="interval" name="interval" class="form-control" placeholder="30" min="5">
                <span>seconds</span>
              </div>
            </div>

                <button type="submit" class="btn btn-primary btn-block">Save Configuration</button>
              </form>
            </div>

            <div class="card">
              <div class="card-title">MQTT Topics</div>

              <div style="margin-bottom: 1.5rem;">
                <h4 style="margin-bottom: 0.5rem; color: var(--text-primary);">Subscriber Topics</h4>
                <div id="sublist" class="alert" style="font-size: 0.875rem; background: var(--surface); border-color: var(--border);">Loading...</div>
              </div>

              <div>
                <h4 style="margin-bottom: 0.5rem; color: var(--text-primary);">Publisher Topics</h4>
                <div id="publist" class="alert" style="font-size: 0.875rem; background: var(--surface); border-color: var(--border);">Loading...</div>
              </div>
            </div>
          </div>
        </section>

        <!-- Scheduler Section -->
        <section id="scheduler-section" class="content-section">
          <div class="dashboard-grid">
            <div class="card">
              <div class="card-title">Weekly Schedule</div>
              <div class="form-group">
                <div class="checkbox-group">
                  <input type="checkbox" id="schedulerEnabled" class="checkbox" onchange="saveSchedulerEnabled()">
                  <label for="schedulerEnabled" class="form-label">Enable scheduler (applies each half-hour, overrides general settings)</label>
                </div>
              </div>
              <p style="font-size:0.8rem;color:var(--text-secondary);margin-bottom:0.75rem">Select a rule on the right, then click/drag cells to paint them. Click a painted cell again to erase it.</p>
              <div id="schedGrid" class="sched-grid-wrap"><div class="loading">Loading...</div></div>
            </div>
            <div class="card sched-editor">
              <div class="card-title">Rules</div>
              <div id="schedRulesList" style="margin-bottom:0.4rem"></div>
              <button class="btn btn-secondary btn-block" onclick="schedAddRule()" style="margin-bottom:0.4rem">+ New rule</button>
              <hr style="border:none;border-top:1px solid var(--border);margin-bottom:0.4rem">
              <div class="form-group">
                <label class="form-label">Display mode</label>
                <select id="schedMode" class="form-control" onchange="schedEditorChange()"></select>
              </div>
              <div class="form-group">
                <label class="form-label">Color</label>
                <input type="color" id="schedColor" value="#ffffff" class="form-control" style="height:2rem;padding:0.1rem;cursor:pointer" onchange="schedEditorChange()">
              </div>
              <div class="form-group">
                <label class="form-label">Color randomization</label>
                <select id="schedColorRandom" class="form-control" onchange="schedEditorChange()">
                  <option value="0">No Random</option>
                  <option value="1">Random all</option>
                  <option value="2">Random letter</option>
                  <option value="3">Random word</option>
                </select>
              </div>
              <div class="form-group">
                <label class="form-label">Animation</label>
                <select id="schedAnimation" class="form-control" onchange="schedEditorChange()"></select>
              </div>
              <div class="form-group">
                <label class="form-label">Speed (1=slow, 20=fast)</label>
                <div class="range-group">
                  <input type="range" id="schedAnimSpeed" class="range-input" min="1" max="20" step="1" oninput="document.getElementById('schedAnimSpeedT').textContent=this.value;schedEditorChange()">
                  <div class="range-value" id="schedAnimSpeedT">10</div>
                </div>
              </div>
              <div class="form-group">
                <label class="form-label">Min brightness (%)</label>
                <div class="range-group">
                  <input type="range" id="schedAnimBrightMin" class="range-input" min="0" max="100" step="1" oninput="document.getElementById('schedAnimBrightMinT').textContent=this.value;schedEditorChange()">
                  <div class="range-value" id="schedAnimBrightMinT">10</div>
                </div>
              </div>
              <div class="form-group">
                <label class="form-label">Max brightness (%)</label>
                <div class="range-group">
                  <input type="range" id="schedAnimBrightMax" class="range-input" min="0" max="100" step="1" oninput="document.getElementById('schedAnimBrightMaxT').textContent=this.value;schedEditorChange()">
                  <div class="range-value" id="schedAnimBrightMaxT">100</div>
                </div>
              </div>
              <div style="display:flex;gap:0.5rem;margin-top:0.5rem">
                <button class="btn btn-secondary" onclick="schedRaz()" style="flex:1">RAZ</button>
                <button class="btn btn-primary" id="schedSaveBtn" onclick="saveAllScheduler()" style="flex:2">Save</button>
              </div>
            </div>
          </div>
        </section>

        <!-- Update Section -->
        <section id="update-section" class="content-section">
          <div class="dashboard-grid">
            <div class="card">
              <div class="card-title">Firmware Update</div>
              <p style="margin-bottom: 1.5rem; color: var(--text-secondary);">Upload a new firmware file to update your TexTime device.</p>

              <div class="alert alert-warning" style="margin-bottom: 1.5rem;">
                <strong>Warning:</strong> Only upload firmware files specifically designed for TexTime.
                Uploading incorrect firmware can permanently damage your device.
              </div>

              <form action="/update" method="post" enctype="multipart/form-data">
                <div class="form-group">
                  <label for="firmware" class="form-label">Select firmware file (.bin)</label>
                  <input type="file" id="firmware" name="firmware" class="form-control" accept=".bin" required>
                </div>

                <button type="submit" class="btn btn-primary btn-block">Upload Firmware</button>
              </form>
            </div>
          </div>
        </section>

      </div>
    </div>

    <div class="footer">
      <a href="https://github.com/psykokwak-com/TexTime">By Psykokwak</a>
    </div>
  </div>

  <!-- Mobile overlay -->
  <div class="overlay" id="overlay"></div>

  <script>
    // Custom color picker — HSV square + hue slider, no native <input type="color">
    var _cpH=0, _cpS=1, _cpV=1, _cpInit=false;

    function _cpHsvToHex(h,s,v) {
      var i=Math.floor(h/60)%6, f=h/60-Math.floor(h/60);
      var p=v*(1-s), q=v*(1-f*s), t=v*(1-(1-f)*s);
      var rgb=[[v,t,p],[q,v,p],[p,v,t],[p,q,v],[t,p,v],[v,p,q]][i];
      return '#'+rgb.map(function(x){return Math.round(x*255).toString(16).padStart(2,'0');}).join('');
    }

    function _cpHexToHsv(hex) {
      var r=parseInt(hex.slice(1,3),16)/255, g=parseInt(hex.slice(3,5),16)/255, b=parseInt(hex.slice(5,7),16)/255;
      var max=Math.max(r,g,b), min=Math.min(r,g,b), d=max-min, h=0;
      if(d){if(max===r)h=((g-b)/d+(g<b?6:0))*60; else if(max===g)h=((b-r)/d+2)*60; else h=((r-g)/d+4)*60;}
      return {h:h, s:max?d/max:0, v:max};
    }

    function _cpApply(send) {
      var hex=_cpHsvToHex(_cpH,_cpS,_cpV);
      var sw=document.getElementById('colorSwatch');
      var ct=document.getElementById('colorText');
      var ci=document.getElementById('color');
      var sq=document.getElementById('svSquare');
      var th=document.getElementById('svThumb');
      var hs=document.getElementById('hueSlider');
      if(sw) sw.style.backgroundColor=hex;
      if(ct) ct.value=hex;
      if(ci) ci.value=hex;
      if(sq) sq.style.backgroundColor='hsl('+Math.round(_cpH)+',100%,50%)';
      if(th){th.style.left=(_cpS*100)+'%'; th.style.top=((1-_cpV)*100)+'%';}
      if(hs) hs.value=_cpH;
      if(send) updatecolor(hex);
    }

    function _cpSetFromHex(hex) {
      if(!isValidHexColor(hex)) return;
      var hsv=_cpHexToHsv(hex);
      _cpH=hsv.h; _cpS=hsv.s; _cpV=hsv.v;
      _cpApply(false);
    }

    function initColorPicker() {
      if(_cpInit) {
        var ct=document.getElementById('colorText');
        if(ct && ct.value) _cpSetFromHex(ct.value.trim().startsWith('#')?ct.value.trim():'#'+ct.value.trim());
        return;
      }
      _cpInit=true;
      var swatch=document.getElementById('colorSwatch');
      var panel=document.getElementById('cpickerPanel');
      var sq=document.getElementById('svSquare');
      var hs=document.getElementById('hueSlider');
      var ct=document.getElementById('colorText');
      var panelOpen=false;

      swatch.addEventListener('click', function(e) {
        e.stopPropagation();
        panelOpen=!panelOpen;
        panel.style.display=panelOpen?'block':'none';
      });
      document.addEventListener('click', function(e) {
        if(panelOpen && !panel.contains(e.target) && e.target!==swatch) {
          panelOpen=false; panel.style.display='none';
        }
      });

      var svDrag=false;
      function onSV(e) {
        e.preventDefault();
        var rect=sq.getBoundingClientRect();
        var cx=e.touches?e.touches[0].clientX:e.clientX;
        var cy=e.touches?e.touches[0].clientY:e.clientY;
        _cpS=Math.max(0,Math.min(1,(cx-rect.left)/rect.width));
        _cpV=Math.max(0,Math.min(1,1-(cy-rect.top)/rect.height));
        _cpApply(true);
      }
      sq.addEventListener('mousedown',  function(e){svDrag=true; onSV(e);});
      sq.addEventListener('touchstart', function(e){svDrag=true; onSV(e);},{passive:false});
      document.addEventListener('mousemove',  function(e){if(svDrag) onSV(e);});
      document.addEventListener('touchmove',  function(e){if(svDrag) onSV(e);},{passive:false});
      document.addEventListener('mouseup',    function(){svDrag=false;});
      document.addEventListener('touchend',   function(){svDrag=false;});

      hs.addEventListener('input', function(){_cpH=parseInt(this.value); _cpApply(true);});

      ct.addEventListener('input', function() {
        var v=this.value.trim();
        if(!v.startsWith('#')) v='#'+v;
        if(isValidHexColor(v)){_cpSetFromHex(v); updatecolor(v);}
      });
    }

    // Dashboard Management
    let currentSection = 'info';
    let updateIntervals = {};

    // Section titles mapping
    const sectionTitles = {
      'info': 'System Information',
      'general': 'General Settings',
      'network': 'Network Configuration',
      'ntp': 'Time Configuration',
      'mqtt': 'MQTT Configuration',
      'update': 'Firmware Update',
      'scheduler': 'Display Scheduler'
    };

    // Utility functions
    function debounce(func, wait) {
      let timeout;
      return function executedFunction(...args) {
        const later = () => {
          clearTimeout(timeout);
          func(...args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
      };
    }

    // Navigation Management
    function switchSection(sectionName) {
      // Update navigation
      document.querySelectorAll('.nav-item').forEach(btn => {
        btn.classList.remove('active');
      });
      document.querySelector(`[data-section="${sectionName}"]`).classList.add('active');

      // Update content
      document.querySelectorAll('.content-section').forEach(section => {
        section.classList.remove('active');
      });
      document.getElementById(`${sectionName}-section`).classList.add('active');

      // Update header title
      document.getElementById('sectionTitle').textContent = sectionTitles[sectionName] || 'Dashboard';

      // Stop previous section updates
      if (updateIntervals[currentSection]) {
        clearInterval(updateIntervals[currentSection]);
        delete updateIntervals[currentSection];
      }

      currentSection = sectionName;

      // Load section-specific data
      loadSectionData(sectionName);

      // Close mobile menu
      closeMobileMenu();
    }

    // Mobile menu functions
    function toggleMobileMenu() {
      const sidebar = document.getElementById('sidebar');
      const overlay = document.getElementById('overlay');

      sidebar.classList.toggle('open');
      overlay.classList.toggle('active');
    }

    function closeMobileMenu() {
      const sidebar = document.getElementById('sidebar');
      const overlay = document.getElementById('overlay');

      sidebar.classList.remove('open');
      overlay.classList.remove('active');
    }

    // Theme toggle
    function toggleTheme() {
      const html = document.documentElement;
      const themeToggle = document.getElementById('themeToggle');

      if (html.getAttribute('data-theme') === 'dark') {
        html.setAttribute('data-theme', 'light');
        themeToggle.textContent = '🌙';
        localStorage.setItem('theme', 'light');
      } else {
        html.setAttribute('data-theme', 'dark');
        themeToggle.textContent = '☀️';
        localStorage.setItem('theme', 'dark');
      }
    }

    // Load saved theme
    function loadTheme() {
      const savedTheme = localStorage.getItem('theme') || 'light';
      const html = document.documentElement;
      const themeToggle = document.getElementById('themeToggle');

      html.setAttribute('data-theme', savedTheme);
      themeToggle.textContent = savedTheme === 'dark' ? '☀️' : '🌙';
    }

    function showLoading(elementId) {
      const element = document.getElementById(elementId);
      if (element) {
        element.innerHTML = '<div class="loading">Loading...</div>';
      }
    }

    function hideLoading(elementId) {
      const element = document.getElementById(elementId);
      if (element && element.querySelector('.loading')) {
        element.innerHTML = '';
      }
    }

    // Load data for specific section
    function loadSectionData(sectionName) {
      switch(sectionName) {
        case 'info':
          loadSystemInfo();
          break;
        case 'general':
          loadGeneralSettings();
          break;
        case 'network':
          loadNetworkSettings();
          break;
        case 'ntp':
          loadNtpSettings();
          break;
        case 'mqtt':
          loadMqttSettings();
          break;
        case 'scheduler':
          loadSchedulerSettings();
          break;
      }
    }

    // Network display improvements
    function displayNetworks(networks) {
      const networksContainer = document.getElementById('networks');
      if (!networks || networks.length === 0) {
        networksContainer.innerHTML = '<div class="loading">No networks found</div>';
        return;
      }

      // Remove duplicates based on SSID
      const uniqueNetworks = networks.filter((network, index, self) =>
        index === self.findIndex(n => n.ssid === network.ssid)
      );

      const networksHTML = uniqueNetworks.map(network => {
        const signalBars = getSignalBars(network.rssi);
        const isConnected = network.connected || false;
        const security = network.security || 'Open';

        return `
          <div class="network-item ${isConnected ? 'connected' : ''}" onclick="selssid('${network.ssid}')">
            <div class="network-info">
              <div class="network-name">${network.ssid}</div>
              <div class="network-details">
                <div class="network-signal">
                  <div class="signal-bars">${signalBars}</div>
                  <span>${network.rssi}%</span>
                </div>
                <div class="network-security">
                  <span class="security-icon">${security === 'Open' ? '🔓' : '🔒'}</span>
                  <span>${security}</span>
                </div>
              </div>
            </div>
            <div class="network-actions">
              ${isConnected ?
                '<span class="connected-badge">Connected</span>' :
                '<button class="connect-btn" onclick="event.stopPropagation(); selssid(\'${network.ssid}\')">Connect</button>'
              }
            </div>
          </div>
        `;
      }).join('');

      networksContainer.innerHTML = networksHTML;
    }

    function getSignalBars(rssi) {
      const strength = Math.max(0, Math.min(100, rssi));
      const bars = Math.ceil(strength / 20);

      return Array.from({length: 5}, (_, i) =>
        `<div class="signal-bar ${i < bars ? 'active' : ''}"></div>`
      ).join('');
    }

    // Fix dropdown duplicates
    function removeDuplicateOptions(selectElement) {
      const options = Array.from(selectElement.options);
      const uniqueOptions = options.filter((option, index, self) =>
        index === self.findIndex(o => o.value === option.value)
      );

      selectElement.innerHTML = '';
      uniqueOptions.forEach(option => selectElement.appendChild(option));
    }

    // System Info functions
    function loadSystemInfo() {
      getSystemState();
      updateIntervals.info = setInterval(getSystemState, 1000);
    }

    function getSystemState() {
      setValuesHighPriority("/admin/infovalues");
    }

    // General Settings functions
    const debouncedUpdate = debounce((param, value) => {
      setValues(`/admin/led?${param}=${value}`);
    }, 300);

    function updatebrightness() {
      const value = document.getElementById("brightness").value;
      document.getElementById("brightnesst").textContent = value;
      debouncedUpdate("brightness", value);
    }

    function updatebrightnessday() {
      const value = document.getElementById("brightnessday").value;
      document.getElementById("brightnessdayt").textContent = value;
      debouncedUpdate("brightnessday", value);
    }

    function updatebrightnessnight() {
      const value = document.getElementById("brightnessnight").value;
      document.getElementById("brightnessnightt").textContent = value;
      debouncedUpdate("brightnessnight", value);
    }

    function updatecolor(colorValue) {
      // Remove # if present for API call
      const cleanColor = colorValue.replace('#', '');
      debouncedUpdate("color", cleanColor);
    }

    function initializeColorPicker() {
      initColorPicker();
      var ct=document.getElementById('colorText');
      if(ct && ct.value) {
        var v=ct.value.trim();
        if(!v.startsWith('#')) v='#'+v;
        _cpSetFromHex(v);
      }
    }

    function hexToRgb(hex) {
      const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
      return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
      } : null;
    }

    function isValidHexColor(hex) {
      return /^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/.test(hex);
    }

    function updatecolorrandom() {
      setValues("/admin/led?colorrandom=" + document.getElementById("colorrandom").value);
    }

    function updatelang() {
      setValues("/admin/led?lang=" + document.getElementById("lang").value);
    }

    function updatemode() {
      setValues("/admin/led?mode=" + document.getElementById("mode").value);
    }

    function updateanimation() {
      setValues("/admin/led?animation=" + document.getElementById("animation").value);
    }

    function updateanimspeed() {
      const value = document.getElementById("animspeed").value;
      document.getElementById("animspeedt").textContent = value;
      debouncedUpdate("animspeed", value);
    }

    function updateanimbrightmin() {
      const value = document.getElementById("animbrightmin").value;
      document.getElementById("animbrightmint").textContent = value;
      debouncedUpdate("animbrightmin", value);
    }

    function updateanimbrightmax() {
      const value = document.getElementById("animbrightmax").value;
      document.getElementById("animbrightmaxt").textContent = value;
      debouncedUpdate("animbrightmax", value);
    }

    function validatebrightnessauto() {
      const isAuto = document.getElementById('brightnessauto').checked;

      document.getElementById("brightness").disabled = isAuto;
      document.getElementById("brightnessday").disabled = !isAuto;
      document.getElementById("brightnessnight").disabled = !isAuto;
      document.getElementById("brightnesssensibility").disabled = !isAuto;

      document.getElementById("brightnesst").textContent = document.getElementById("brightness").value;
      document.getElementById("brightnessdayt").textContent = document.getElementById("brightnessday").value;
      document.getElementById("brightnessnightt").textContent = document.getElementById("brightnessnight").value;
    }

    function updatebrightnesssensibility() {
      setValues("/admin/led?brightnesssensibility=" + document.getElementById("brightnesssensibility").value);
    }

    function updatebrightnessauto() {
      validatebrightnessauto();
      if (document.getElementById('brightnessauto').checked) {
        setValues("/admin/led?brightness=-1");
      } else {
        updatebrightness();
      }
    }

    // Utility function for save button feedback
    function setSaveButtonState(button, state) {
      if (!button) {
        return;
      }

      // Store original text if not already stored
      if (!button.dataset.originalText) {
        button.dataset.originalText = button.textContent;
      }
      const originalText = button.dataset.originalText;

      button.disabled = true;

      if (state === 'saving') {
        button.textContent = 'Saving...';
        button.style.backgroundColor = '#ffa500';
        button.style.color = 'white';
      } else if (state === 'success') {
        button.textContent = '✓ Saved';
        button.style.backgroundColor = '#28a745';
        button.style.color = 'white';
        setTimeout(() => {
          button.textContent = originalText;
          button.style.backgroundColor = '';
          button.style.color = '';
          button.disabled = false;
        }, 3000);
      } else if (state === 'error') {
        button.textContent = '✗ Error';
        button.style.backgroundColor = '#dc3545';
        button.style.color = 'white';
        setTimeout(() => {
          button.textContent = originalText;
          button.style.backgroundColor = '';
          button.style.color = '';
          button.disabled = false;
        }, 3000);
      }
    }

    function saveGeneralSettings(event) {
      event.preventDefault();
      const form = event.target;
      const button = form.querySelector('button[type="submit"]') || form.querySelector('button') || event.submitter;
      const formData = new FormData(form);

      setSaveButtonState(button, 'saving');

      fetch('/admin/save/general', {
        method: 'POST',
        body: formData
      })
      .then(response => {
        if (response.ok) {
          setSaveButtonState(button, 'success');
          // Reload values to reflect changes
          loadGeneralSettings();
        } else {
          setSaveButtonState(button, 'error');
        }
      })
      .catch(error => {
        setSaveButtonState(button, 'error');
      });

      return false;
    }

    // Network Settings functions
    function loadNetworkSettings() {
      setValues("/admin/networkfieldsvalues")
        .then(() => {
          validateNetworkSettings();
          return refreshNetworks();
        })
        .then(() => {
          // Initial network status update
          updateNetworkConnectionStatus();
        });
    }

    function validateNetworkSettings() {
      const isDHCP = document.getElementById('dhcp').checked;
      const staticConfig = document.getElementById('static-config');

      if (isDHCP) {
        staticConfig.style.opacity = '0.5';
        staticConfig.style.pointerEvents = 'none';
      } else {
        staticConfig.style.opacity = '1';
        staticConfig.style.pointerEvents = 'auto';
      }

      ['ipaddress', 'netmask', 'gateway', 'dnsserver'].forEach(id => {
        const element = document.getElementById(id);
        if (element) {
          element.disabled = isDHCP;
        }
      });
    }

    function refreshNetworks() {
      showLoading('networks');
      return setValues("/admin/networkconnectionvalues")
        .then(() => {
          hideLoading('networks');
          // Update network connection status display
          updateNetworkConnectionStatus();
        })
        .catch(error => {
          console.error('Failed to get network state:', error);
          document.getElementById('networks').innerHTML = '<div class="alert alert-error">Failed to scan networks</div>';
        });
    }

    function updateNetworkConnectionStatus() {
      const connectionStateRaw = document.getElementById('connectionstate');
      const connectionStateDisplay = document.getElementById('connectionstatedisplay');

      if (connectionStateRaw && connectionStateDisplay && connectionStateRaw.textContent) {
        const status = connectionStateRaw.textContent.trim();

        // Remove existing status classes from display element
        connectionStateDisplay.classList.remove('connected', 'connecting', 'disconnected');

        // Add appropriate status class and update display (keeping raw data intact)
        if (status === 'CONNECTED') {
          connectionStateDisplay.classList.add('status-indicator', 'connected');
          connectionStateDisplay.innerHTML = '✅ Connected';
        } else if (status === 'SCAN COMPLETED' || status === 'Checking...') {
          connectionStateDisplay.classList.add('status-indicator', 'connecting');
          connectionStateDisplay.innerHTML = '🔍 Scanning...';
        } else if (status === 'DISCONNECTED' || status === 'CONNECTION LOST') {
          connectionStateDisplay.classList.add('status-indicator', 'disconnected');
          connectionStateDisplay.innerHTML = '❌ Disconnected';
        } else if (status === 'CONNECT FAILED') {
          connectionStateDisplay.classList.add('status-indicator', 'disconnected');
          connectionStateDisplay.innerHTML = '⚠️ Connection Failed';
        } else if (status === 'NO SSID AVAILBLE') {
          connectionStateDisplay.classList.add('status-indicator', 'disconnected');
          connectionStateDisplay.innerHTML = '📡 No Network Available';
        } else if (status === 'Idle') {
          connectionStateDisplay.classList.add('status-indicator', 'connecting');
          connectionStateDisplay.innerHTML = '⏳ Idle';
        } else {
          connectionStateDisplay.classList.add('status-indicator', 'disconnected');
          connectionStateDisplay.innerHTML = '🔄 ' + status;
        }
      }
    }

    function loadGeneralSettings() {
      setValues("/admin/generalledconfigvalues")
        .then(() => setValues("/admin/generallangsvalues"))
        .then(() => setValues("/admin/generalmodesvalues"))
        .then(() => setValues("/admin/generalanimationsvalues"))
        .then(() => setValues("/admin/generalfieldsvalues"))
        .then(() => {
          validatebrightnessauto();
          // Remove duplicates from dropdowns
          removeDuplicateOptions(document.getElementById('ledconfig'));
          removeDuplicateOptions(document.getElementById('lang'));
          removeDuplicateOptions(document.getElementById('mode'));
          removeDuplicateOptions(document.getElementById('animation'));
          // Sync slider display values (setValues doesn't fire oninput)
          document.getElementById('animspeedt').textContent = document.getElementById('animspeed').value;
          document.getElementById('animbrightmint').textContent = document.getElementById('animbrightmin').value;
          document.getElementById('animbrightmaxt').textContent = document.getElementById('animbrightmax').value;
          // Initialize color picker after values are loaded
          initializeColorPicker();
        });
    }

    function selssid(value) {
      document.getElementById("ssid").value = value;
    }

    function saveNetworkSettings(event) {
      event.preventDefault();
      const form = event.target;
      const button = form.querySelector('button[type="submit"]') || form.querySelector('button') || event.submitter;
      const formData = new FormData(form);

      setSaveButtonState(button, 'saving');

      fetch('/admin/save/network', {
        method: 'POST',
        body: formData
      })
      .then(response => {
        if (response.ok) {
          setSaveButtonState(button, 'success');
          // Network settings save triggers ESP restart, so show success and warn user
          setTimeout(() => {
            alert('Network settings saved! Device is restarting...');
          }, 1000);
        } else {
          setSaveButtonState(button, 'error');
        }
      })
      .catch(error => {
        setSaveButtonState(button, 'error');
      });

      return false;
    }

    // NTP Settings functions
    function loadNtpSettings() {
      setValues("/admin/ntpfieldsvalues");
    }

    function saveNtpSettings(event) {
      event.preventDefault();
      const form = event.target;
      const button = form.querySelector('button[type="submit"]') || form.querySelector('button') || event.submitter;
      const formData = new FormData(form);

      setSaveButtonState(button, 'saving');

      fetch('/admin/save/ntp', {
        method: 'POST',
        body: formData
      })
      .then(response => {
        if (response.ok) {
          setSaveButtonState(button, 'success');
          // Reload values to reflect changes
          loadNtpSettings();
        } else {
          setSaveButtonState(button, 'error');
        }
      })
      .catch(error => {
        setSaveButtonState(button, 'error');
      });

      return false;
    }

    // MQTT Settings functions
    function loadMqttSettings() {
      setValues("/admin/mqttfieldsvalues")
        .then(() => {
          getMqttState();
          updateIntervals.mqtt = setInterval(getMqttState, 3000);
        });
    }

    function getMqttState() {
      setValuesHighPriority("/admin/mqttconnectionvalues")
        .then(() => {
          // Update MQTT connection status display with visual indicator
          updateMqttConnectionStatus();
        });
    }

    function updateMqttConnectionStatus() {
      const connectionState = document.getElementById('mqttconnectionstate');
      if (connectionState && connectionState.textContent) {
        const status = connectionState.textContent.trim();

        // Remove existing status classes
        connectionState.classList.remove('connected', 'connecting', 'disconnected');

        // Add appropriate status class and update display
        if (status === 'MQTT_CONNECTED') {
          connectionState.classList.add('status-indicator', 'connected');
          connectionState.innerHTML = '✅ Connected';
        } else if (status.includes('CONNECTING') || status === 'Checking...') {
          connectionState.classList.add('status-indicator', 'connecting');
          connectionState.innerHTML = '🔄 Connecting...';
        } else {
          connectionState.classList.add('status-indicator', 'disconnected');
          connectionState.innerHTML = '❌ ' + status.replace('MQTT_', '').replace('_', ' ');
        }
      }
    }

    function saveMqttSettings(event) {
      event.preventDefault();
      const form = event.target;
      const button = form.querySelector('button[type="submit"]') || form.querySelector('button') || event.submitter;
      const formData = new FormData(form);

      setSaveButtonState(button, 'saving');

      fetch('/admin/save/mqtt', {
        method: 'POST',
        body: formData
      })
      .then(response => {
        if (response.ok) {
          setSaveButtonState(button, 'success');
          // Reload values to reflect changes
          loadMqttSettings();
        } else {
          setSaveButtonState(button, 'error');
        }
      })
      .catch(error => {
        setSaveButtonState(button, 'error');
      });

      return false;
    }

    // Load function
    function load(src, type, callback) {
      if (type === 'js') {
        const script = document.createElement('script');
        script.src = src;
        script.type = 'text/javascript';
        script.async = false;
        script.onload = () => callback && callback();
        script.onerror = () => console.error(`Failed to load script: ${src}`);
        document.head.appendChild(script);
      } else if (type === 'css') {
        const link = document.createElement('link');
        link.href = src;
        link.rel = 'stylesheet';
        link.type = 'text/css';
        link.onload = () => callback && callback();
        link.onerror = () => console.error(`Failed to load stylesheet: ${src}`);
        document.head.appendChild(link);
      }
    }

    // Initialize app
    window.onload = function() {
      load("style.css?t="+Date.now(), "css", function() {
        // Load saved theme
        loadTheme();

        // Setup navigation
        document.querySelectorAll('.nav-item').forEach(btn => {
          btn.addEventListener('click', () => {
            switchSection(btn.dataset.section);
          });
        });

        // Setup mobile menu
        const menuToggle = document.getElementById('menuToggle');
        const overlay = document.getElementById('overlay');
        const themeToggle = document.getElementById('themeToggle');

        menuToggle.addEventListener('click', toggleMobileMenu);
        overlay.addEventListener('click', closeMobileMenu);
        themeToggle.addEventListener('click', toggleTheme);

        // Load initial section
        loadSectionData(currentSection);

        // Initialize color picker
        initializeColorPicker();

        // Set initial header title
        document.getElementById('sectionTitle').textContent = sectionTitles[currentSection] || 'Dashboard';

        console.log("TexTime dashboard loaded");
      });
    }

    // Cleanup intervals when leaving page
    window.addEventListener('beforeunload', () => {
      Object.values(updateIntervals).forEach(interval => {
        clearInterval(interval);
      });
    });

    // Handle visibility change to pause/resume updates
    window.addEventListener('visibilitychange', function() {
      if (document.hidden) {
        Object.values(updateIntervals).forEach(interval => {
          clearInterval(interval);
        });
        updateIntervals = {};
      } else {
        loadSectionData(currentSection);
      }
    });

    // Handle escape key to close mobile menu
    document.addEventListener('keydown', function(event) {
      if (event.key === 'Escape') {
        closeMobileMenu();
      }
    });

    // Handle window resize
    window.addEventListener('resize', function() {
      if (window.innerWidth >= 1024) {
        closeMobileMenu();
      }
    });

    // ── Scheduler ──────────────────────────────────────────────────────────────
    function toHex2(v){var h=Math.round(v).toString(16);return h.length<2?'0'+h:h;}
    function schedTextColor(r,g,b){return (r*299+g*587+b*114)/1000>128?'#111':'#fff';}
    var schedCells=new Array(336).fill(null);
    var schedRules=[];
    var schedCurrentRule=0;
    var schedModeNames=[],schedAnimNames=[];
    var schedDragging=false,schedDragMode=null;

    function schedEditorChange() {
      if(!schedRules.length) return;
      var r=schedRules[schedCurrentRule];
      r.mode=parseInt(document.getElementById('schedMode').value);
      r.anim=parseInt(document.getElementById('schedAnimation').value);
      r.colorRandom=parseInt(document.getElementById('schedColorRandom').value);
      r.animSpeed=parseInt(document.getElementById('schedAnimSpeed').value);
      r.animBrightMin=parseInt(document.getElementById('schedAnimBrightMin').value);
      r.animBrightMax=parseInt(document.getElementById('schedAnimBrightMax').value);
      var ch=document.getElementById('schedColor').value.replace('#','');
      r.r=parseInt(ch.substr(0,2),16)||0;
      r.g=parseInt(ch.substr(2,2),16)||0;
      r.b=parseInt(ch.substr(4,2),16)||0;
      schedRenderRulesList();
      var hex='#'+toHex2(r.r)+toHex2(r.g)+toHex2(r.b);
      var tc=schedTextColor(r.r,r.g,r.b);
      document.querySelectorAll('#schedGrid .sched-half.sched-cur').forEach(function(el){
        el.style.background=hex; el.style.color=tc;
      });
    }

    function schedLoadEditor(rule) {
      document.getElementById('schedMode').value=rule.mode;
      document.getElementById('schedColorRandom').value=rule.colorRandom;
      document.getElementById('schedAnimation').value=rule.anim;
      document.getElementById('schedAnimSpeed').value=rule.animSpeed;
      document.getElementById('schedAnimSpeedT').textContent=rule.animSpeed;
      document.getElementById('schedAnimBrightMin').value=rule.animBrightMin;
      document.getElementById('schedAnimBrightMinT').textContent=rule.animBrightMin;
      document.getElementById('schedAnimBrightMax').value=rule.animBrightMax;
      document.getElementById('schedAnimBrightMaxT').textContent=rule.animBrightMax;
      document.getElementById('schedColor').value='#'+toHex2(rule.r)+toHex2(rule.g)+toHex2(rule.b);
    }

    function schedRenderRulesList() {
      var el=document.getElementById('schedRulesList');
      var crNames=['no random','rnd all','rnd letter','rnd word'];
      var html='';
      schedRules.forEach(function(rule,i){
        var ledHex='#'+toHex2(rule.r)+toHex2(rule.g)+toHex2(rule.b);
        var mname=schedModeNames[rule.mode]||'mode '+rule.mode;
        var crname=crNames[rule.colorRandom]||'';
        var aname=schedAnimNames[rule.anim]||'anim '+rule.anim;
        var label=mname+' - '+crname+' - '+aname;
        var active=i===schedCurrentRule;
        html+='<div class="sched-rule-item'+(active?' sched-rule-active':'')+'" onclick="schedSelectRule('+i+')">'
          +'<div class="sched-rule-num">'+(i+1)+'</div>'
          +'<div style="flex:1;font-size:0.8rem">'+label+'</div>'
          +'<div class="sched-rule-led" style="background:'+ledHex+'" title="LED color"></div>'
          +'<button class="sched-rule-del" onclick="event.stopPropagation();schedDelRule('+i+')" title="Delete">&#x2715;</button>'
          +'</div>';
      });
      el.innerHTML=html;
    }

    function schedSelectRule(idx) {
      schedCurrentRule=idx;
      schedLoadEditor(schedRules[idx]);
      schedRenderRulesList();
      renderSchedulerGrid();
    }

    function schedAddRule() {
      schedRules.push({mode:0,anim:0,colorRandom:0,animSpeed:10,animBrightMin:10,animBrightMax:100,r:0,g:0,b:0});
      schedSelectRule(schedRules.length-1);
    }

    function schedDelRule(idx) {
      if(schedRules.length<=1) return;
      schedRules.splice(idx,1);
      for(var i=0;i<336;i++){
        if(schedCells[i]===idx) schedCells[i]=null;
        else if(schedCells[i]>idx) schedCells[i]--;
      }
      if(schedCurrentRule>=schedRules.length) schedCurrentRule=schedRules.length-1;
      schedLoadEditor(schedRules[schedCurrentRule]);
      schedRenderRulesList();
      renderSchedulerGrid();
    }

    function renderSchedulerGrid() {
      var days=['Mon','Tue','Wed','Thu','Fri','Sat','Sun'];
      var html='<table class="sched-table"><thead><tr><th></th>';
      days.forEach(function(d){html+='<th>'+d+'</th>';});
      html+='</tr></thead><tbody>';
      for(var h=0;h<24;h++){
        html+='<tr><td class="sched-hour">'+(h<10?'0':'')+h+'h</td>';
        for(var d=0;d<7;d++){
          html+='<td class="sched-cell">';
          for(var m=0;m<2;m++){
            var idx=d*48+h*2+m;
            var ri=schedCells[idx];
            var style='',label='';
            if(ri!==null&&ri!==undefined){
              var rr=schedRules[ri];
              style='background:#'+toHex2(rr.r)+toHex2(rr.g)+toHex2(rr.b)+';color:'+schedTextColor(rr.r,rr.g,rr.b)+';';
              label=String(ri+1);
            }
            var isCur=(ri===schedCurrentRule)?'sched-cur':'';
            html+='<div class="sched-half '+isCur+'" data-d="'+d+'" data-h="'+h+'" data-m="'+(m*30)+'" style="'+style+'">'+label+'</div>';
          }
          html+='</td>';
        }
        html+='</tr>';
      }
      html+='</tbody></table>';
      document.getElementById('schedGrid').innerHTML=html;
      schedInitGridEvents();
    }

    function schedCellAt(el) {
      var c=el&&el.closest?el.closest('[data-d]'):null;
      if(!c||!c.classList.contains('sched-half')) return null;
      var d=parseInt(c.dataset.d),h=parseInt(c.dataset.h),m=parseInt(c.dataset.m);
      return {el:c,idx:d*48+h*2+(m>=30?1:0)};
    }

    function schedPaintEl(info) {
      var ri=schedCells[info.idx];
      if(ri!==null&&ri!==undefined){
        var rr=schedRules[ri];
        info.el.style.background='#'+toHex2(rr.r)+toHex2(rr.g)+toHex2(rr.b);
        info.el.style.color=schedTextColor(rr.r,rr.g,rr.b);
        info.el.textContent=String(ri+1);
        info.el.className='sched-half'+(ri===schedCurrentRule?' sched-cur':'');
      } else {
        info.el.style.cssText='';
        info.el.textContent='';
        info.el.className='sched-half';
      }
    }

    function schedInitGridEvents() {
      var grid=document.getElementById('schedGrid');
      if(!grid||grid._ei) return;
      grid._ei=true;
      function ptInfo(x,y){return schedCellAt(document.elementFromPoint(x,y));}
      function onStart(x,y){
        var info=ptInfo(x,y); if(!info) return;
        schedDragging=true;
        schedDragMode=(schedCells[info.idx]===schedCurrentRule)?'remove':'add';
        schedCells[info.idx]=schedDragMode==='add'?schedCurrentRule:null;
        schedPaintEl(info);
      }
      function onMove(x,y){
        if(!schedDragging) return;
        var info=ptInfo(x,y); if(!info) return;
        if(schedDragMode==='add'&&schedCells[info.idx]!==schedCurrentRule){
          schedCells[info.idx]=schedCurrentRule; schedPaintEl(info);
        } else if(schedDragMode==='remove'&&schedCells[info.idx]!==null){
          schedCells[info.idx]=null; schedPaintEl(info);
        }
      }
      function onEnd(){schedDragging=false;schedDragMode=null;}
      grid.addEventListener('mousedown',function(e){onStart(e.clientX,e.clientY);e.preventDefault();});
      document.addEventListener('mousemove',function(e){onMove(e.clientX,e.clientY);});
      document.addEventListener('mouseup',onEnd);
      grid.addEventListener('touchstart',function(e){onStart(e.touches[0].clientX,e.touches[0].clientY);e.preventDefault();},{passive:false});
      document.addEventListener('touchmove',function(e){if(schedDragging){onMove(e.touches[0].clientX,e.touches[0].clientY);e.preventDefault();}},{passive:false});
      document.addEventListener('touchend',onEnd);
    }

    function schedRaz() {
      if(!confirm('Clear all scheduled slots?')) return;
      schedCells=new Array(336).fill(null);
      renderSchedulerGrid();
    }

    function loadSchedulerSettings() {
      setValues('/admin/schedulerconfig')
        .then(function(){
          return fetch('/admin/generalmodesvalues').then(function(r){return r.text();}).then(function(txt){
            schedModeNames=[];
            txt.split('\n').forEach(function(l){var p=l.split('|');if(p.length>=2&&p[1])schedModeNames.push(p[1]);});
          });
        })
        .then(function(){
          return fetch('/admin/generalanimationsvalues').then(function(r){return r.text();}).then(function(txt){
            schedAnimNames=[];
            txt.split('\n').forEach(function(l){var p=l.split('|');if(p.length>=2&&p[1])schedAnimNames.push(p[1]);});
            var ms=document.getElementById('schedMode');ms.innerHTML='';
            schedModeNames.forEach(function(n,i){var o=document.createElement('option');o.value=i;o.textContent=n;ms.appendChild(o);});
            var as=document.getElementById('schedAnimation');as.innerHTML='';
            schedAnimNames.forEach(function(n,i){var o=document.createElement('option');o.value=i;o.textContent=n;as.appendChild(o);});
          });
        })
        .then(function(){
          return fetch('/admin/schedulerdata').then(function(r){return r.text();}).then(function(txt){
            schedCells=new Array(336).fill(null);
            schedRules=[];
            var ruleMap={};
            for(var i=0;i<336;i++){
              var hex=txt.substr(i*16,16);
              if(hex.length<16) continue;
              var b0=parseInt(hex.substr(0,2),16);
              if(b0===0xFF) continue;
              var key=hex;
              if(ruleMap[key]===undefined){
                var b1=parseInt(hex.substr(2,2),16);
                var b2=parseInt(hex.substr(4,2),16);
                ruleMap[key]=schedRules.length;
                schedRules.push({
                  mode:b0,anim:b1,
                  colorRandom:b2&0x03,animSpeed:((b2>>2)&0x1F)+1,
                  animBrightMin:parseInt(hex.substr(6,2),16),
                  animBrightMax:parseInt(hex.substr(8,2),16),
                  r:parseInt(hex.substr(10,2),16),
                  g:parseInt(hex.substr(12,2),16),
                  b:parseInt(hex.substr(14,2),16)
                });
              }
              schedCells[i]=ruleMap[key];
            }
            if(!schedRules.length) schedRules.push({mode:1,anim:0,colorRandom:0,animSpeed:10,animBrightMin:10,animBrightMax:100,r:255,g:255,b:255});
            schedCurrentRule=0;
            schedLoadEditor(schedRules[0]);
            schedRenderRulesList();
            renderSchedulerGrid();
          });
        });
    }

    function saveSchedulerEnabled() {
      var en=document.getElementById('schedulerEnabled').checked?'1':'0';
      fetch('/admin/save/schedulerenabled',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'enabled='+en});
    }

    async function saveAllScheduler() {
      var btn=document.getElementById('schedSaveBtn');
      setSaveButtonState(btn,'saving');
      try {
        var hex='';
        for(var i=0;i<336;i++){
          var ri=schedCells[i];
          if(ri===null||ri===undefined){
            hex+='FFFFFFFFFFFFFFFF';
          } else {
            var rule=schedRules[ri];
            var packed=(rule.colorRandom&0x03)|((rule.animSpeed-1)<<2);
            hex+=toHex2(rule.mode)+toHex2(rule.anim)+toHex2(packed)
                +toHex2(rule.animBrightMin)+toHex2(rule.animBrightMax)
                +toHex2(rule.r)+toHex2(rule.g)+toHex2(rule.b);
          }
        }
        await fetch('/admin/save/schedulerbulk',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'data='+hex});
        await fetch('/admin/scheduler/apply',{method:'POST'});
        setSaveButtonState(btn,'success');
      } catch(e){setSaveButtonState(btn,'error');}
    }
    // ── End Scheduler ──────────────────────────────────────────────────────────

  </script>
</body>
</html>
)=====";
