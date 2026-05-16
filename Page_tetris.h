const char PAGE_tetris[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style>
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent;}
html,body{margin:0;padding:0;width:100%;height:100%;background:#111;color:#eee;font-family:sans-serif;overflow:hidden;}
#wrap{display:flex;flex-direction:column;height:100%;padding:6px;gap:6px;}
#topbar{display:flex;align-items:center;gap:6px;flex-shrink:0;}
#score{flex:1;font-size:13px;text-align:right;color:#aaa;}
#status{font-size:12px;color:#fa0;text-align:center;flex-shrink:0;}
#gamearea{display:flex;flex:1;gap:6px;min-height:0;}
.col{flex:1;display:flex;flex-direction:column;align-items:center;justify-content:center;gap:8px;}
.btn{background:#222;color:#fff;border:2px solid #444;border-radius:10px;
  cursor:pointer;touch-action:manipulation;user-select:none;-webkit-user-select:none;
  display:flex;align-items:center;justify-content:center;font-size:15px;}
.btn:active{background:#444;}
.tbtn{height:40px;padding:0 10px;font-size:13px;}
.start{border-color:#2a5;background:#163;}
.exit{border-color:#844;background:#422;}
.dpad-row{display:flex;gap:8px;width:100%;}
.dpad-btn{flex:1;height:110px;}
.dpad-down{width:100%;height:72px;}
.rotate-btn{width:140px;height:140px;border-color:#06c;}
.btn svg{display:block;margin:auto;}
#nextlabel{font-size:11px;color:#888;margin-bottom:2px;text-align:center;width:100%;}
#nextcanvas{border:1px solid #333;background:#000;border-radius:6px;display:block;margin:0 auto;}
</style>
<div id="wrap">
  <div id="topbar">
    <div class="btn tbtn start" ontouchstart="doStart(event)" onmousedown="doStart(event)">Start</div>
    <div class="btn tbtn exit"  ontouchstart="doExit(event)"  onmousedown="doExit(event)">Exit</div>
    <div id="status">READY</div>
    <div id="score">Score: <b id="sc">0</b> | Lines: <b id="ln">0</b> | Lv.<b id="lv">0</b></div>
  </div>
  <div id="gamearea">
    <div class="col" style="flex:1.6">
      <div class="dpad-row">
        <div class="btn dpad-btn" ontouchstart="hT('left',event)"  ontouchend="rel(event)"  ontouchcancel="rel(event)"  onmousedown="hM('left',this)"  onmouseup="relM()" onmouseleave="relM()"><svg width="44" height="44" viewBox="0 0 24 24"><polygon points="18,3 18,21 4,12" fill="white"/></svg></div>
        <div class="btn dpad-btn" ontouchstart="hT('right',event)" ontouchend="rel(event)"  ontouchcancel="rel(event)"  onmousedown="hM('right',this)" onmouseup="relM()" onmouseleave="relM()"><svg width="44" height="44" viewBox="0 0 24 24"><polygon points="6,3 6,21 20,12" fill="white"/></svg></div>
      </div>
      <div class="btn dpad-down" ontouchstart="hT('down',event)" ontouchend="rel(event)" ontouchcancel="rel(event)" onmousedown="hM('down',this)" onmouseup="relM()" onmouseleave="relM()"><svg width="44" height="44" viewBox="0 0 24 24"><polygon points="3,6 21,6 12,20" fill="white"/></svg></div>
    </div>
    <div class="col" style="flex:1.4">
      <div id="nextlabel">NEXT</div>
      <canvas id="nextcanvas" width="180" height="180"></canvas>
    </div>
    <div class="col" style="flex:0.8">
      <div class="btn rotate-btn" ontouchstart="aT('rotate',event)" onmousedown="aMD('rotate')"><svg width="80" height="80" viewBox="0 0 24 24" fill="none" stroke="white" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="23 4 23 10 17 10"/><path d="M20.5 15a9 9 0 1 1-3-7.7L23 10"/></svg></div>
    </div>
  </div>
</div>
<script>
var _ht=null;
var SHAPES=[
  [[1,1,1,1],[0,0,0,0],[0,0,0,0],[0,0,0,0]],
  [[0,1,1,0],[0,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[0,1,0,0],[1,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[0,1,1,0],[1,1,0,0],[0,0,0,0],[0,0,0,0]],
  [[1,1,0,0],[0,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[1,0,0,0],[1,1,1,0],[0,0,0,0],[0,0,0,0]],
  [[0,0,1,0],[1,1,1,0],[0,0,0,0],[0,0,0,0]]
];
var COLORS=['#0CC','#CC0','#90C','#0C0','#C00','#00C','#C60'];
function req(u){var x=new XMLHttpRequest();x.open('GET',u,true);x.send();return x;}
function aT(a,e){e.preventDefault();req('/admin/tetris?action='+a);}
function aMD(a){req('/admin/tetris?action='+a);}
function hT(a,e){e.preventDefault();req('/admin/tetris?action='+a);_ht=setInterval(function(){req('/admin/tetris?action='+a);},150);}
function hM(a,b){req('/admin/tetris?action='+a);_ht=setInterval(function(){req('/admin/tetris?action='+a);},150);}
function rel(e){e.preventDefault();if(_ht){clearInterval(_ht);_ht=null;}}
function relM(){if(_ht){clearInterval(_ht);_ht=null;}}
function doStart(e){e.preventDefault();req('/admin/tetris?action=start');}
function doExit(e){e.preventDefault();req('/admin/tetris?action=exit');setTimeout(function(){window.location='/';},200);}
function drawNext(i){
  var cv=document.getElementById('nextcanvas');
  if(!cv)return;
  var ctx=cv.getContext('2d'),sz=cv.width/4;
  ctx.clearRect(0,0,cv.width,cv.height);
  if(i<0||i>6)return;
  ctx.fillStyle=COLORS[i];
  var s=SHAPES[i];
  var minR=4,maxR=-1,minC=4,maxC=-1;
  for(var r=0;r<4;r++)for(var c=0;c<4;c++)
    if(s[r][c]){if(r<minR)minR=r;if(r>maxR)maxR=r;if(c<minC)minC=c;if(c>maxC)maxC=c;}
  var ox=(cv.width-(maxC-minC+1)*sz)/2;
  var oy=(cv.height-(maxR-minR+1)*sz)/2;
  for(var r=0;r<4;r++)for(var c=0;c<4;c++)
    if(s[r][c])ctx.fillRect(ox+(c-minC)*sz+1,oy+(r-minR)*sz+1,sz-2,sz-2);
}
function getState(){
  var x=new XMLHttpRequest();
  x.onreadystatechange=function(){
    if(x.readyState!=4||x.status!=200)return;
    x.responseText.split('\n').forEach(function(l){
      var p=l.split('|');if(p.length<2)return;
      if(p[0]=='sc'||p[0]=='ln'||p[0]=='lv'){var e=document.getElementById(p[0]);if(e)e.innerHTML=p[1];}
      else if(p[0]=='status'){document.getElementById('status').innerHTML=p[1];}
      else if(p[0]=='next'){drawNext(parseInt(p[1]));}
    });
  };
  x.open('GET','/admin/tetrisstate',true);x.send();
}
window.onload=function(){
  load('style.css','css',function(){
    req('/admin/tetris?action=start');
    setInterval(getState,400);
  });
};
function load(e,t,n){if('css'==t){var a=document.createElement('link');a.href=e;a.rel='stylesheet';a.type='text/css';a.async=!1;a.onload=function(){n()};document.getElementsByTagName('head')[0].appendChild(a)}}
var _kr={};
document.addEventListener('keydown',function(e){
  if(e.repeat)return;
  if(e.key==='ArrowLeft'){e.preventDefault();req('/admin/tetris?action=left');_kr.left=setInterval(function(){req('/admin/tetris?action=left');},150);}
  else if(e.key==='ArrowRight'){e.preventDefault();req('/admin/tetris?action=right');_kr.right=setInterval(function(){req('/admin/tetris?action=right');},150);}
  else if(e.key==='ArrowDown'){e.preventDefault();req('/admin/tetris?action=down');_kr.down=setInterval(function(){req('/admin/tetris?action=down');},150);}
  else if(e.key==='ArrowUp'||e.key===' '||e.key==='z'||e.key==='a'){e.preventDefault();req('/admin/tetris?action=rotate');}
});
document.addEventListener('keyup',function(e){
  if(e.key==='ArrowLeft'&&_kr.left){clearInterval(_kr.left);_kr.left=null;}
  if(e.key==='ArrowRight'&&_kr.right){clearInterval(_kr.right);_kr.right=null;}
  if(e.key==='ArrowDown'&&_kr.down){clearInterval(_kr.down);_kr.down=null;}
});
var _gp={},_gpR={};
function pollGP(){
  var now=Date.now(),gs=navigator.getGamepads?navigator.getGamepads():[];
  for(var i=0;i<gs.length;i++){
    var g=gs[i];if(!g)continue;
    [0,1,12].forEach(function(b){var p=g.buttons[b]&&g.buttons[b].pressed,k=i+'b'+b;if(p&&!_gp[k])req('/admin/tetris?action=rotate');_gp[k]=p;});
    [{b:14,a:'left'},{b:15,a:'right'},{b:13,a:'down'}].forEach(function(m){
      var p=g.buttons[m.b]&&g.buttons[m.b].pressed,k=i+'b'+m.b;
      if(p){if(!_gp[k]){req('/admin/tetris?action='+m.a);_gpR[k]=now+200;}else if(now>=(_gpR[k]||0)){req('/admin/tetris?action='+m.a);_gpR[k]=now+150;}}
      _gp[k]=p;
    });
    var ax=g.axes[0]||0,ay=g.axes[1]||0,h=ax<-0.5?'left':ax>0.5?'right':'';
    if(h){if(!_gp['h'+i]){req('/admin/tetris?action='+h);_gpR['h'+i]=now+200;}else if(now>=(_gpR['h'+i]||0)){req('/admin/tetris?action='+h);_gpR['h'+i]=now+150;}}
    _gp['h'+i]=h;
    var vd=ay>0.5;if(vd){if(!_gp['v'+i]){req('/admin/tetris?action=down');_gpR['v'+i]=now+200;}else if(now>=(_gpR['v'+i]||0)){req('/admin/tetris?action=down');_gpR['v'+i]=now+150;}}
    _gp['v'+i]=vd;
    var vu=ay<-0.5;if(vu&&!_gp['vu'+i])req('/admin/tetris?action=rotate');_gp['vu'+i]=vu;
  }
  requestAnimationFrame(pollGP);
}
window.addEventListener('gamepadconnected',function(){pollGP();});
</script>
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
