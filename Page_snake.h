// Same idea as the Tetris page: the screen is the clock on the wall, so this
// page is a gamepad, not a game view. See Page_tetris.h for the reasoning.
//
// Snake only needs a direction, never a repeat, so a press fires once. The
// accent green and the food orange are the literal RGB values the firmware
// writes to the strip in LedStripAnimationSnake::renderBoard().
const char PAGE_snake[] PROGMEM = R"=====(<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover,user-scalable=no">
<title>Snake</title>
<style>
:root{
  --bg:#0f172a; --panel:#1e293b; --raise:#243449; --edge:#334155;
  --ink:#f1f5f9; --muted:#94a3b8; --dim:#64748b;
  --green:#00ff50; --food:#ff5000; --danger:#ef4444;
  --accent:#00ff50;
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
.title{font-size:var(--legend);letter-spacing:.3em;text-transform:uppercase;color:var(--green);font-weight:700}
.state{display:flex;align-items:center;gap:.45em;font-size:var(--legend);letter-spacing:.14em;text-transform:uppercase;color:var(--muted);font-weight:600}
.dot{width:.5em;height:.5em;border-radius:50%;background:var(--dim);box-shadow:0 0 8px currentColor;color:var(--dim)}
.hud[data-s="PLAYING"] .dot{background:var(--green);color:var(--green)}
.hud[data-s="GAME OVER"] .dot{background:var(--danger);color:var(--danger)}
.hud[data-s="NO LINK"] .dot{background:var(--danger);color:var(--danger);opacity:.5}
.readout{margin-left:auto;text-align:right}
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
.link:focus-visible{outline:2px solid var(--green);outline-offset:2px}

/* ---- Deck ------------------------------------------------------------ */
.deck{flex:1 1 auto;min-height:0;display:flex;align-items:center;justify-content:center;padding:var(--gap)}
.pad{
  display:grid;gap:var(--gap);
  grid-template-columns:repeat(3,1fr);grid-template-rows:repeat(3,1fr);
  aspect-ratio:1;width:min(100%,440px,74vh);
}
/* Portrait: sink the cross into the thumb zone instead of floating it. */
@media (max-aspect-ratio:1/1){
  .deck{align-items:flex-end}
}

/* ---- Keys ------------------------------------------------------------ */
.key{
  border:1px solid var(--edge);border-radius:var(--r);color:var(--muted);
  background:radial-gradient(130% 130% at 50% 0%,var(--raise) 0%,var(--panel) 62%);
  display:flex;align-items:center;justify-content:center;
  cursor:pointer;touch-action:none;user-select:none;-webkit-user-select:none;
  font:inherit;padding:0;min-height:0;min-width:0;
  transition:color .16s ease,border-color .16s ease,box-shadow .16s ease,transform .07s ease;
}
.key svg{width:clamp(24px,9vmin,60px);height:auto;display:block;fill:currentColor}
/* Pressed state, whatever the input device. The bloom imitates a diffused
   pixel lighting up behind the clock's faceplate. */
.key.on{
  color:var(--ink);border-color:var(--green);transform:scale(.96);
  box-shadow:0 0 0 1px var(--green) inset,0 0 20px -4px var(--green),0 0 48px -10px var(--green);
}
.k-up{grid-area:1/2}.k-left{grid-area:2/1}.k-right{grid-area:2/3}.k-down{grid-area:3/2}
@media (prefers-reduced-motion:reduce){.key{transition:none}.key.on{transform:none}}

/* Ended or disconnected: the controls no longer do anything, so stop them
   looking as though they might, and point at the way back. */
body.dead .deck{opacity:.35;pointer-events:none}
body.dead .key{box-shadow:none}
.link.cta{background:var(--panel);color:var(--ink);border-color:var(--accent);box-shadow:0 0 16px -6px var(--accent)}

/* Centre of the cross: one pixel, lit the way the head is lit on the wall. */
.core{
  grid-area:2/2;display:flex;align-items:center;justify-content:center;
}
.core i{
  width:38%;aspect-ratio:1;border-radius:50%;background:var(--dim);
  box-shadow:0 0 14px -2px var(--dim);transition:background .25s ease,box-shadow .25s ease;
}
.hud[data-s="PLAYING"]~.deck .core i{background:var(--green);box-shadow:0 0 22px -2px var(--green)}
.hud[data-s="GAME OVER"]~.deck .core i{background:var(--danger);box-shadow:0 0 22px -2px var(--danger)}
</style>
</head>
<body>

<header class="hud" id="hud" data-s="READY">
  <span class="title">Snake</span>
  <span class="state"><i class="dot"></i><span id="status">Ready</span></span>
  <dl class="readout"><dt>Score</dt><dd id="sc">0</dd></dl>
  <button class="link" id="restart">Restart</button>
  <button class="link" id="quit">Back to clock</button>
</header>

<main class="deck">
  <section class="pad">
    <button type="button" tabindex="-1" class="key k-up"    data-a="up"    aria-label="Go up"><svg viewBox="0 0 24 24"><polygon points="3,18 21,18 12,5"/></svg></button>
    <button type="button" tabindex="-1" class="key k-left"  data-a="left"  aria-label="Go left"><svg viewBox="0 0 24 24"><polygon points="18,3 18,21 5,12"/></svg></button>
    <div class="core"><i></i></div>
    <button type="button" tabindex="-1" class="key k-right" data-a="right" aria-label="Go right"><svg viewBox="0 0 24 24"><polygon points="6,3 6,21 19,12"/></svg></button>
    <button type="button" tabindex="-1" class="key k-down"  data-a="down"  aria-label="Go down"><svg viewBox="0 0 24 24"><polygon points="3,6 21,6 12,19"/></svg></button>
  </section>
</main>

<script>
var URL_ACT='/admin/snake?action=',URL_ST='/admin/snakestate';
var DIRS=['up','down','left','right'];
var held={},fails=0;

function send(a){var x=new XMLHttpRequest();x.open('GET',URL_ACT+a,true);x.send();}
function keyEl(a){return document.querySelector('.key[data-a="'+a+'"]');}

/* One entry point for every input device. A direction is a state, not a
   repeated action, so a press fires exactly once however long it is held. */
function press(a){
  if(held[a])return;
  held[a]=1;
  var e=keyEl(a);if(e)e.classList.add('on');
  if(navigator.vibrate){try{navigator.vibrate(8);}catch(_){}}
  send(a);
}
function release(a){
  if(!held[a])return;
  held[a]=0;
  var e=keyEl(a);if(e)e.classList.remove('on');
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

var KEYS={ArrowUp:'up',ArrowDown:'down',ArrowLeft:'left',ArrowRight:'right',
          w:'up',W:'up',s:'down',S:'down',a:'left',A:'left',d:'right',D:'right'};
document.addEventListener('keydown',function(e){
  var a=KEYS[e.key];if(!a)return;
  e.preventDefault();press(a);
});
document.addEventListener('keyup',function(e){
  var a=KEYS[e.key];if(a)release(a);
});
window.addEventListener('blur',releaseAll);

/* Gamepad: d-pad and left stick steer. */
var gpOn={},gpLoop=0;
function pollGP(){
  var gs=navigator.getGamepads?navigator.getGamepads():[],now={};
  for(var i=0;i<gs.length;i++){
    var g=gs[i];if(!g)continue;
    var bp=function(n){return g.buttons[n]&&g.buttons[n].pressed;};
    if(bp(12))now.up=1; if(bp(13))now.down=1; if(bp(14))now.left=1; if(bp(15))now.right=1;
    var ax=g.axes[0]||0,ay=g.axes[1]||0;
    if(ax<-.5)now.left=1; if(ax>.5)now.right=1;
    if(ay<-.5)now.up=1;   if(ay>.5)now.down=1;
  }
  DIRS.forEach(function(a){
    if(now[a]&&!gpOn[a])press(a);
    else if(!now[a]&&gpOn[a])release(a);
  });
  gpOn=now;
  requestAnimationFrame(pollGP);
}
window.addEventListener('gamepadconnected',function(){if(!gpLoop){gpLoop=1;pollGP();}});

var LABEL={'READY':'Ready','PLAYING':'Playing','GAME OVER':'Game over','ENDED':'Ended','NO LINK':'No link'};
function setStatus(s){
  document.getElementById('hud').dataset.s=s;
  document.getElementById('status').textContent=LABEL[s]||s;
  /* The game can be ended from elsewhere -- the other game, a mode change,
     the idle timeout. Say so and point at the way back rather than leaving a
     live-looking score above buttons that no longer do anything. */
  var dead=(s==='ENDED'||s==='NO LINK');
  document.body.classList.toggle('dead',dead);
  var r=document.getElementById('restart');
  if(r) r.classList.toggle('cta',s==='ENDED');
}
function getState(){
  var x=new XMLHttpRequest();
  x.onreadystatechange=function(){
    if(x.readyState!=4)return;
    if(x.status!=200){if(++fails>2)setStatus('NO LINK');return;}
    fails=0;
    x.responseText.split('\n').forEach(function(l){
      var p=l.split('|');if(p.length<2)return;
      if(p[0]=='sc'){var e=document.getElementById('sc');if(e)e.textContent=p[1];}
      else if(p[0]=='status')setStatus(p[1]);
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

void send_snake_html()
{
  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");
  _server.send_P(200, "text/html", PAGE_snake);
}

void send_snake_action()
{
  String a = _server.arg("action");

  if (a == "exit") {
    QTLed.snakeStop();
    _server.send(200, "text/plain", "exit");
    return;
  }

  if (a == "start") {
    if (!QTLed.isSnakeActive()) QTLed.snakeStart();
    else if (LedStripAnimationSnake::instance)
      LedStripAnimationSnake::instance->action(LedStripAnimationSnake::ACT_START);
    QTLed.gameKeepAlive();
    _server.send(200, "text/plain", "ok");
    return;
  }

  if (LedStripAnimationSnake::instance == nullptr || !QTLed.isSnakeActive()) {
    _server.send(200, "text/plain", "inactive");
    return;
  }

  // Only a real move from a real player counts. Deliberately placed after the
  // active check: a key pressed on a stale page belonging to the other game
  // must not extend the session of the game that is actually running.
  QTLed.gameKeepAlive();

  LedStripAnimationSnake::SnaAction act = LedStripAnimationSnake::ACT_NONE;
  if      (a == "up")    act = LedStripAnimationSnake::ACT_UP;
  else if (a == "down")  act = LedStripAnimationSnake::ACT_DOWN;
  else if (a == "left")  act = LedStripAnimationSnake::ACT_LEFT;
  else if (a == "right") act = LedStripAnimationSnake::ACT_RIGHT;
  LedStripAnimationSnake::instance->action(act);
  _server.send(200, "text/plain", "ok");
}

void send_snake_state()
{
  String s = "";
  if (LedStripAnimationSnake::instance) {
    LedStripAnimationSnake *t = LedStripAnimationSnake::instance;
    s += "sc|"     + String(t->getScore()) + "\n";

    // The game object keeps its last state after being stopped, so report
    // whether it still owns the display. Without this a page whose game was
    // ended elsewhere -- by the other game, a mode change or the idle timeout
    // -- kept showing a live score above buttons that no longer did anything.
    if (!QTLed.isSnakeActive())
      s += "status|ENDED\n";
    else switch (t->getState()) {
      case LedStripAnimationSnake::STATE_PLAYING:   s += "status|PLAYING\n";   break;
      case LedStripAnimationSnake::STATE_GAME_OVER: s += "status|GAME OVER\n"; break;
      default:                                      s += "status|READY\n";     break;
    }
  }
  _server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _server.sendHeader("Pragma", "no-cache");
  _server.sendHeader("Expires", "-1");
  _server.send(200, "text/plain", s);
}
