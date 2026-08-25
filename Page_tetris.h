// The screen for this game is the clock on the wall, not the browser: this page
// is a gamepad. Everything below follows from that.
//
//  - The player is looking at the wall, so the controls never move between
//    layouts (d-pad always bottom-left, actions always bottom-right) and every
//    input method -- touch, mouse, keyboard, gamepad -- lights the same on-screen
//    key, so a glance down is enough to confirm what was pressed.
//  - Accent colours are the literal RGB values the firmware writes to the strip
//    (TET_COLORS below), so the controller is colour-matched to the wall.
//  - Surfaces reuse the dashboard's dark palette so this reads as the same
//    device rather than a separate app.
//  - Self-contained: no external stylesheet fetch, which also removes a round
//    trip from a web server that has a game loop to run.
const char PAGE_tetris[] PROGMEM = R"=====(<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover,user-scalable=no">
<title>Tetris</title>
<style>
:root{
  --bg:#0f172a; --panel:#1e293b; --raise:#243449; --edge:#334155;
  --ink:#f1f5f9; --muted:#94a3b8; --dim:#64748b;
  --cyan:#00cccc; --violet:#a855f7; --amber:#e08a00; --danger:#ef4444;
  --r:14px;
  --gap:clamp(6px,1.6vmin,14px);
  --legend:clamp(9px,1.5vmin,11px);
}
*{box-sizing:border-box;margin:0;padding:0;-webkit-tap-highlight-color:transparent}
html,body{height:100%;overflow:hidden}
body{
  background:var(--bg);color:var(--ink);
  font-family:system-ui,-apple-system,"Segoe UI",Roboto,sans-serif;
  display:flex;flex-direction:column;
  padding:env(safe-area-inset-top) env(safe-area-inset-right) env(safe-area-inset-bottom) env(safe-area-inset-left);
}
.legend{
  font-size:var(--legend);letter-spacing:.14em;text-transform:uppercase;
  color:var(--dim);font-weight:600;
}

/* ---- HUD ------------------------------------------------------------- */
.hud{
  flex:0 0 auto;display:flex;align-items:center;gap:var(--gap);flex-wrap:wrap;
  padding:calc(var(--gap)*.9) var(--gap);border-bottom:1px solid var(--edge);
}
.title{font-size:var(--legend);letter-spacing:.3em;text-transform:uppercase;color:var(--cyan);font-weight:700}
.state{display:flex;align-items:center;gap:.45em;font-size:var(--legend);letter-spacing:.14em;text-transform:uppercase;color:var(--muted);font-weight:600}
.dot{width:.5em;height:.5em;border-radius:50%;background:var(--dim);box-shadow:0 0 8px currentColor;color:var(--dim)}
.hud[data-s="PLAYING"] .dot{background:var(--cyan);color:var(--cyan)}
.hud[data-s="GAME OVER"] .dot{background:var(--danger);color:var(--danger)}
.hud[data-s="NO LINK"] .dot{background:var(--danger);color:var(--danger);opacity:.5}
.readout{margin-left:auto;display:flex;gap:clamp(10px,2.4vmin,22px)}
.readout div{text-align:right}
.readout dt{font-size:var(--legend);letter-spacing:.14em;text-transform:uppercase;color:var(--dim);font-weight:600}
.readout dd{
  font-family:ui-monospace,SFMono-Regular,"SF Mono",Menlo,Consolas,monospace;
  font-variant-numeric:tabular-nums;font-size:clamp(15px,3.2vmin,24px);line-height:1.1;
}
.link{
  background:var(--panel);color:var(--muted);border:1px solid var(--edge);
  border-radius:9px;padding:.5em .85em;font:inherit;font-size:var(--legend);
  letter-spacing:.1em;text-transform:uppercase;font-weight:600;cursor:pointer;
}
.link:hover{background:var(--raise);color:var(--ink)}
.link:focus-visible{outline:2px solid var(--cyan);outline-offset:2px}

/* ---- Deck ------------------------------------------------------------ */
.deck{
  flex:1 1 auto;min-height:0;display:grid;gap:var(--gap);padding:var(--gap);
  grid-template-columns:1fr auto 1fr;grid-template-areas:"pad next act";
  align-items:center;
}
.pad{grid-area:pad;display:grid;gap:var(--gap);grid-template-columns:1fr 1fr;grid-template-rows:1fr .72fr;height:100%}
.act{grid-area:act;display:grid;gap:var(--gap);grid-template-rows:1fr .72fr;height:100%}
.next{grid-area:next;display:flex;flex-direction:column;align-items:center;gap:calc(var(--gap)*.7);padding:0 var(--gap)}
#nextcanvas{
  width:clamp(64px,15vmin,124px);height:auto;aspect-ratio:1;
  background:#070c14;border:1px solid var(--edge);border-radius:10px;
}

/* Portrait: the whole controller drops into the thumb zone, pad and actions
   keep their left/right sides so muscle memory survives the rotation. */
@media (max-aspect-ratio:1/1){
  .deck{grid-template-columns:1fr 1fr;grid-template-rows:auto 1fr;grid-template-areas:"next next" "pad act"}
  .next{flex-direction:row;justify-content:center}
}

/* ---- Keys ------------------------------------------------------------ */
.key{
  border:1px solid var(--edge);border-radius:var(--r);color:var(--muted);
  background:radial-gradient(130% 130% at 50% 0%,var(--raise) 0%,var(--panel) 62%);
  display:flex;flex-direction:column;align-items:center;justify-content:center;gap:.4em;
  cursor:pointer;touch-action:none;user-select:none;-webkit-user-select:none;
  font:inherit;padding:0;min-height:0;min-width:0;
  transition:color .16s ease,border-color .16s ease,box-shadow .16s ease,transform .07s ease;
}
.key svg{width:clamp(22px,7vmin,52px);height:auto;display:block;fill:currentColor}
/* Pressed state, whatever the input device. The bloom imitates a diffused
   pixel lighting up behind the clock's faceplate. */
.key.on{
  color:var(--ink);border-color:var(--glow);transform:scale(.97);
  box-shadow:0 0 0 1px var(--glow) inset,0 0 20px -4px var(--glow),0 0 48px -10px var(--glow);
}
.k-left,.k-right,.k-down{--glow:var(--cyan)}
.k-rotate{--glow:var(--violet)}
.k-drop{--glow:var(--amber)}
.k-down{grid-column:1/-1}
@media (prefers-reduced-motion:reduce){.key{transition:none}.key.on{transform:none}}
</style>
</head>
<body>

<header class="hud" id="hud" data-s="READY">
  <span class="title">Tetris</span>
  <span class="state"><i class="dot"></i><span id="status">Ready</span></span>
  <dl class="readout">
    <div><dt>Score</dt><dd id="sc">0</dd></div>
    <div><dt>Lines</dt><dd id="ln">0</dd></div>
    <div><dt>Level</dt><dd id="lv">0</dd></div>
  </dl>
  <button class="link" id="restart">Restart</button>
  <button class="link" id="quit">Back to clock</button>
</header>

<main class="deck">
  <section class="pad">
    <button type="button" tabindex="-1" class="key k-left"  data-a="left"  aria-label="Move left"><svg viewBox="0 0 24 24"><polygon points="17,3 17,21 5,12"/></svg><span class="legend">Left</span></button>
    <button type="button" tabindex="-1" class="key k-right" data-a="right" aria-label="Move right"><svg viewBox="0 0 24 24"><polygon points="7,3 7,21 19,12"/></svg><span class="legend">Right</span></button>
    <button type="button" tabindex="-1" class="key k-down"  data-a="down"  aria-label="Move down"><svg viewBox="0 0 24 24"><polygon points="3,7 21,7 12,19"/></svg><span class="legend">Soft drop</span></button>
  </section>

  <section class="next">
    <span class="legend">Next</span>
    <canvas id="nextcanvas" width="120" height="120"></canvas>
  </section>

  <section class="act">
    <button type="button" tabindex="-1" class="key k-rotate" data-a="rotate" aria-label="Rotate"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><polyline points="22 5 22 11 16 11"/><path d="M19.5 15.5a8 8 0 1 1-2.6-7.2L22 11"/></svg><span class="legend">Rotate</span></button>
    <button type="button" tabindex="-1" class="key k-drop"   data-a="drop"   aria-label="Hard drop"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 3v12"/><polyline points="6 11 12 17 18 11"/><path d="M5 21h14"/></svg><span class="legend">Drop</span></button>
  </section>
</main>

<script>
var URL_ACT='/admin/tetris?action=',URL_ST='/admin/tetrisstate';
var HOLD={left:1,right:1,down:1};        /* keys that auto-repeat while held */
var REPEAT=150;
var held={},timers={},fails=0;

function send(a){var x=new XMLHttpRequest();x.open('GET',URL_ACT+a,true);x.send();}
function keyEl(a){return document.querySelector('.key[data-a="'+a+'"]');}

/* One entry point for every input device, so touch, keyboard and gamepad all
   produce the same request and the same on-screen feedback. */
function press(a){
  if(held[a])return;
  held[a]=1;
  var e=keyEl(a);if(e)e.classList.add('on');
  if(navigator.vibrate){try{navigator.vibrate(8);}catch(_){}}
  send(a);
  if(HOLD[a])timers[a]=setInterval(function(){send(a);},REPEAT);
}
function release(a){
  if(!held[a])return;
  held[a]=0;
  var e=keyEl(a);if(e)e.classList.remove('on');
  if(timers[a]){clearInterval(timers[a]);timers[a]=0;}
}
function releaseAll(){for(var a in held)release(a);}

/* Pointer events cover mouse, touch and pen in one path. */
Array.prototype.forEach.call(document.querySelectorAll('.key'),function(el){
  var a=el.dataset.a;
  el.addEventListener('pointerdown',function(ev){ev.preventDefault();el.setPointerCapture&&el.setPointerCapture(ev.pointerId);press(a);});
  el.addEventListener('pointerup',function(ev){ev.preventDefault();release(a);});
  el.addEventListener('pointercancel',function(){release(a);});
  el.addEventListener('contextmenu',function(ev){ev.preventDefault();});
});

document.getElementById('restart').onclick=function(){send('start');};
document.getElementById('quit').onclick=function(){
  releaseAll();send('exit');setTimeout(function(){window.location='/';},200);
};

/* Give the clock back when this page goes away for good. sendBeacon is the
   only request that survives page teardown. A page merely parked in the back
   /forward cache (persisted) may still come back, so leave it running and let
   the firmware's idle timeout decide. */
window.addEventListener('pagehide',function(e){
  if(e.persisted)return;
  releaseAll();
  if(navigator.sendBeacon)navigator.sendBeacon(URL_ACT+'exit');
});
window.addEventListener('pageshow',function(e){if(e.persisted)getState();});

/* Arrow keys move, Up/Z/X rotate, Space hard-drops (the firmware has always
   supported the drop, no interface ever offered it). */
var KEYS={ArrowLeft:'left',ArrowRight:'right',ArrowDown:'down',
          ArrowUp:'rotate',z:'rotate',Z:'rotate',x:'rotate',X:'rotate',
          ' ':'drop',Spacebar:'drop'};
document.addEventListener('keydown',function(e){
  /* Space activates a focused Restart / Back to clock button; let it. */
  if(e.target&&e.target.classList&&e.target.classList.contains('link'))return;
  var a=KEYS[e.key];if(!a)return;
  e.preventDefault();press(a);
});
document.addEventListener('keyup',function(e){
  var a=KEYS[e.key];if(a)release(a);
});
window.addEventListener('blur',releaseAll);

/* Gamepad: d-pad and left stick move, face buttons rotate, B/circle drops.
   Routed through press/release so holding a direction repeats like a key. */
var gpOn={},gpLoop=0;
function pollGP(){
  var gs=navigator.getGamepads?navigator.getGamepads():[],now={};
  for(var i=0;i<gs.length;i++){
    var g=gs[i];if(!g)continue;
    var bp=function(n){return g.buttons[n]&&g.buttons[n].pressed;};
    if(bp(14))now.left=1; if(bp(15))now.right=1; if(bp(13))now.down=1;
    if(bp(12)||bp(0)||bp(3))now.rotate=1;
    if(bp(1))now.drop=1;
    var ax=g.axes[0]||0,ay=g.axes[1]||0;
    if(ax<-.5)now.left=1; if(ax>.5)now.right=1;
    if(ay>.5)now.down=1;  if(ay<-.5)now.rotate=1;
  }
  ['left','right','down','rotate','drop'].forEach(function(a){
    if(now[a]&&!gpOn[a])press(a);
    else if(!now[a]&&gpOn[a])release(a);
  });
  gpOn=now;
  requestAnimationFrame(pollGP);
}
window.addEventListener('gamepadconnected',function(){if(!gpLoop){gpLoop=1;pollGP();}});

/* NEXT preview, drawn with the exact colours the strip will use. */
var SHAPES=[
  [[1,1,1,1],[0,0,0,0],[0,0,0,0],[0,0,0,0]],
  [[0,1,1,0],[0,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[0,1,0,0],[1,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[0,1,1,0],[1,1,0,0],[0,0,0,0],[0,0,0,0]],
  [[1,1,0,0],[0,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[1,0,0,0],[1,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[0,0,1,0],[1,1,1,0],[0,0,0,0],[0,0,0,0]]
];
var COLORS=['#00c8c8','#c8c800','#9600c8','#00c800','#c80000','#0000c8','#c86400'];
var lastNext=-2;
function drawNext(i){
  if(i===lastNext)return;
  lastNext=i;
  var cv=document.getElementById('nextcanvas');if(!cv)return;
  var ctx=cv.getContext('2d'),sz=cv.width/4;
  ctx.clearRect(0,0,cv.width,cv.height);
  if(i<0||i>6)return;
  var s=SHAPES[i],minR=4,maxR=-1,minC=4,maxC=-1,r,c;
  for(r=0;r<4;r++)for(c=0;c<4;c++)
    if(s[r][c]){if(r<minR)minR=r;if(r>maxR)maxR=r;if(c<minC)minC=c;if(c>maxC)maxC=c;}
  var ox=(cv.width-(maxC-minC+1)*sz)/2,oy=(cv.height-(maxR-minR+1)*sz)/2;
  ctx.fillStyle=COLORS[i];ctx.shadowColor=COLORS[i];ctx.shadowBlur=10;
  for(r=0;r<4;r++)for(c=0;c<4;c++)
    if(s[r][c])ctx.fillRect(ox+(c-minC)*sz+2,oy+(r-minR)*sz+2,sz-4,sz-4);
  ctx.shadowBlur=0;
}

var LABEL={'READY':'Ready','PLAYING':'Playing','GAME OVER':'Game over'};
function setStatus(s){
  document.getElementById('hud').dataset.s=s;
  document.getElementById('status').textContent=LABEL[s]||s;
}
function getState(){
  var x=new XMLHttpRequest();
  x.onreadystatechange=function(){
    if(x.readyState!=4)return;
    if(x.status!=200){if(++fails>2)setStatus('NO LINK');return;}
    fails=0;
    x.responseText.split('\n').forEach(function(l){
      var p=l.split('|');if(p.length<2)return;
      if(p[0]=='sc'||p[0]=='ln'||p[0]=='lv'){var e=document.getElementById(p[0]);if(e)e.textContent=p[1];}
      else if(p[0]=='status')setStatus(p[1]);
      else if(p[0]=='next')drawNext(parseInt(p[1]));
    });
  };
  x.open('GET',URL_ST,true);x.send();
}

send('start');
getState();
setInterval(getState,400);
</script>
</body>
</html>
)=====";

void send_tetris_html()
{
  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");
  _server.send_P(200, "text/html", PAGE_tetris);
}

void send_tetris_action()
{
  QTLed.gameKeepAlive();   // the page is still there

  String a = _server.arg("action");

  if (a == "exit") {
    QTLed.tetrisStop();
    _server.send(200, "text/plain", "exit");
    return;
  }

  if (a == "start") {
    if (!QTLed.isTetrisActive()) QTLed.tetrisStart();
    else if (LedStripAnimationTetris::instance)
      LedStripAnimationTetris::instance->action(LedStripAnimationTetris::ACT_START);
    _server.send(200, "text/plain", "ok");
    return;
  }

  if (LedStripAnimationTetris::instance == nullptr || !QTLed.isTetrisActive()) {
    _server.send(200, "text/plain", "inactive");
    return;
  }

  LedStripAnimationTetris::TetAction act = LedStripAnimationTetris::ACT_NONE;
  if      (a == "left")   act = LedStripAnimationTetris::ACT_LEFT;
  else if (a == "right")  act = LedStripAnimationTetris::ACT_RIGHT;
  else if (a == "rotate") act = LedStripAnimationTetris::ACT_ROTATE;
  else if (a == "down")   act = LedStripAnimationTetris::ACT_DOWN;
  else if (a == "drop")   act = LedStripAnimationTetris::ACT_DROP;
  LedStripAnimationTetris::instance->action(act);
  _server.send(200, "text/plain", "ok");
}

void send_tetris_state()
{
  QTLed.gameKeepAlive();   // the page is still there

  String s = "";
  if (LedStripAnimationTetris::instance) {
    LedStripAnimationTetris *t = LedStripAnimationTetris::instance;
    s += "sc|"     + String(t->getScore()) + "\n";
    s += "ln|"     + String(t->getLines()) + "\n";
    s += "lv|"     + String(t->getLevel()) + "\n";
    s += "next|"   + String(t->getNext())  + "\n";
    switch (t->getState()) {
      case LedStripAnimationTetris::STATE_PLAYING:   s += "status|PLAYING\n";   break;
      case LedStripAnimationTetris::STATE_GAME_OVER: s += "status|GAME OVER\n"; break;
      default:                                       s += "status|READY\n";     break;
    }
  }
  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");
  _server.send(200, "text/plain", s);
}
