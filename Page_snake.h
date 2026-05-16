const char PAGE_snake[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style>
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent;}
html,body{margin:0;padding:0;width:100%;height:100%;background:#111;color:#eee;font-family:sans-serif;overflow:hidden;}
#wrap{display:flex;flex-direction:column;height:100%;padding:6px;gap:6px;}
#topbar{display:flex;align-items:center;gap:6px;flex-shrink:0;}
#score{flex:1;font-size:13px;text-align:right;color:#aaa;}
#status{font-size:12px;color:#fa0;text-align:center;flex-shrink:0;}
#gamearea{display:flex;flex:1;align-items:center;justify-content:center;min-height:0;}
.btn{background:#222;color:#fff;border:2px solid #444;border-radius:10px;
  cursor:pointer;touch-action:manipulation;user-select:none;-webkit-user-select:none;
  display:flex;align-items:center;justify-content:center;}
.btn:active{background:#444;}
.tbtn{height:40px;padding:0 10px;font-size:13px;}
.start{border-color:#2a5;background:#163;}
.exit{border-color:#844;background:#422;}
.btn svg{display:block;margin:auto;}
.dpad{display:grid;grid-template-columns:repeat(3,90px);grid-template-rows:repeat(3,90px);gap:8px;}
.dpad-btn{border-color:#555;}
</style>
<div id="wrap">
  <div id="topbar">
    <div class="btn tbtn start" ontouchstart="doStart(event)" onmousedown="doStart(event)">Start</div>
    <div class="btn tbtn exit"  ontouchstart="doExit(event)"  onmousedown="doExit(event)">Exit</div>
    <div id="status">READY</div>
    <div id="score">Score: <b id="sc">0</b></div>
  </div>
  <div id="gamearea">
    <div class="dpad">
      <div></div>
      <div class="btn dpad-btn" ontouchstart="aT('up',event)"    onmousedown="aMD('up')"   ><svg width="50" height="50" viewBox="0 0 24 24"><polygon points="2,22 22,22 12,2"  fill="white"/></svg></div>
      <div></div>
      <div class="btn dpad-btn" ontouchstart="aT('left',event)"  onmousedown="aMD('left')" ><svg width="50" height="50" viewBox="0 0 24 24"><polygon points="22,2 22,22 2,12"  fill="white"/></svg></div>
      <div></div>
      <div class="btn dpad-btn" ontouchstart="aT('right',event)" onmousedown="aMD('right')"><svg width="50" height="50" viewBox="0 0 24 24"><polygon points="2,2 2,22 22,12"   fill="white"/></svg></div>
      <div></div>
      <div class="btn dpad-btn" ontouchstart="aT('down',event)"  onmousedown="aMD('down')" ><svg width="50" height="50" viewBox="0 0 24 24"><polygon points="2,2 22,2 12,22"   fill="white"/></svg></div>
      <div></div>
    </div>
  </div>
</div>
<script>
function req(u){var x=new XMLHttpRequest();x.open('GET',u,true);x.send();return x;}
function aT(a,e){e.preventDefault();req('/admin/snake?action='+a);}
function aMD(a){req('/admin/snake?action='+a);}
function doStart(e){e.preventDefault();req('/admin/snake?action=start');}
function doExit(e){e.preventDefault();req('/admin/snake?action=exit');setTimeout(function(){window.location='/';},200);}
function getState(){
  var x=new XMLHttpRequest();
  x.onreadystatechange=function(){
    if(x.readyState!=4||x.status!=200)return;
    x.responseText.split('\n').forEach(function(l){
      var p=l.split('|');if(p.length<2)return;
      if(p[0]=='sc'){var e=document.getElementById('sc');if(e)e.innerHTML=p[1];}
      else if(p[0]=='status'){document.getElementById('status').innerHTML=p[1];}
    });
  };
  x.open('GET','/admin/snakestate',true);x.send();
}
window.onload=function(){
  load('style.css','css',function(){
    req('/admin/snake?action=start');
    setInterval(getState,400);
  });
};
function load(e,t,n){if('css'==t){var a=document.createElement('link');a.href=e;a.rel='stylesheet';a.type='text/css';a.async=!1;a.onload=function(){n()};document.getElementsByTagName('head')[0].appendChild(a)}}
document.addEventListener('keydown',function(e){
  var m={'ArrowUp':'up','ArrowDown':'down','ArrowLeft':'left','ArrowRight':'right'};
  if(m[e.key]){e.preventDefault();req('/admin/snake?action='+m[e.key]);}
});
var _gp={};
function pollGP(){
  var gs=navigator.getGamepads?navigator.getGamepads():[];
  for(var i=0;i<gs.length;i++){
    var g=gs[i];if(!g)continue;
    [{b:12,a:'up'},{b:13,a:'down'},{b:14,a:'left'},{b:15,a:'right'}].forEach(function(m){
      var p=g.buttons[m.b]&&g.buttons[m.b].pressed,k=i+'b'+m.b;
      if(p&&!_gp[k])req('/admin/snake?action='+m.a);_gp[k]=p;
    });
    var ax=g.axes[0]||0,ay=g.axes[1]||0;
    var h=ax<-0.5?'left':ax>0.5?'right':'',v=ay<-0.5?'up':ay>0.5?'down':'';
    if(h&&_gp['h'+i]!==h)req('/admin/snake?action='+h);
    if(v&&_gp['v'+i]!==v)req('/admin/snake?action='+v);
    _gp['h'+i]=h;_gp['v'+i]=v;
  }
  requestAnimationFrame(pollGP);
}
window.addEventListener('gamepadconnected',function(){pollGP();});
</script>
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
    _server.send(200, "text/plain", "ok");
    return;
  }

  if (LedStripAnimationSnake::instance == nullptr || !QTLed.isSnakeActive()) {
    _server.send(200, "text/plain", "inactive");
    return;
  }

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
    switch (t->getState()) {
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
