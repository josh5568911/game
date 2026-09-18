;--------------------------------------------------------------------------------
; ティラノスクリプト テーマプラグイン theme_kopanda_20 通常版
; 作者:こ・ぱんだ
; https://kopacurve.blog.fc2.com/
;--------------------------------------------------------------------------------

[iscript]

mp.font_color    = mp.font_color    || "0xF2F2F2";
mp.name_color    = mp.name_color    || "0xF5F5F5";
mp.frame_opacity = mp.frame_opacity || "255";
mp.font_color2   = mp.font_color2   || "0xF2F2F2";
mp.glyph         = mp.glyph         || "on";

if(TG.config.alreadyReadTextColor != "default") {
	TG.config.alreadyReadTextColor = mp.font_color2;
}

[endscript]

; 名前部分のメッセージレイヤ削除
[free name="chara_name_area" layer="message0"]

; メッセージウィンドウの設定
[position layer="message0" width="1280" height="160" top="560" left="0"]
[position layer="message0" frame="../others/plugin/theme_kopanda_20/image/frame_message.png" margint="45" marginl="140" marginr="140" opacity="&mp.frame_opacity" page="fore"]

; 名前枠の設定
[ptext name="chara_name_area" layer="message0" color="&mp.name_color" size="24" x="90" y="560" width="720" align="left"]
[chara_config ptext="chara_name_area"]

; デフォルトのフォントカラー指定
[font color="&mp.font_color"]
[deffont color="&mp.font_color"]

; クリック待ちグリフの設定（on設定時のみ有効）
[if exp="mp.glyph == 'on'"]
[glyph line="../../../data/others/plugin/theme_kopanda_20/image/system/nextpage.png"]
[endif]

;=================================================================================

; システムボタンを表示するマクロ

;=================================================================================
[macro name="add_theme_button"]

[hidemenubutton]

[iscript]

	tf.sysbtn_img_path   = '../others/plugin/theme_kopanda_20/image/button/'; // 画像のパス
	tf.sysbtn_img_width  = 48; // 画像の幅
	tf.sysbtn_img_height = 50; // 画像の高さ
	tf.sysbtn_posx       = [612, 672, 732, 792, 852, 912, 972, 1032, 1092, 1152, 1212, 1188]; // 配置するX座標
	tf.sysbtn_posy       = [508, 20]; // 配置するY座標

[endscript]

; Q.Save
[button name="role_button" role="quicksave" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'qsave.png'" enterimg="&tf.sysbtn_img_path + 'qsave2.png'" x="&tf.sysbtn_posx[0]" y="&tf.sysbtn_posy[0]"]

; Q.Load
[button name="role_button" role="quickload" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'qload.png'" enterimg="&tf.sysbtn_img_path + 'qload2.png'" x="&tf.sysbtn_posx[1]" y="&tf.sysbtn_posy[0]"]

; Save
[button name="role_button" role="save" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'save.png'" enterimg="&tf.sysbtn_img_path + 'save2.png'" x="&tf.sysbtn_posx[2]" y="&tf.sysbtn_posy[0]"]

; Load
[button name="role_button" role="load" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'load.png'" enterimg="&tf.sysbtn_img_path + 'load2.png'" x="&tf.sysbtn_posx[3]" y="&tf.sysbtn_posy[0]"]

; Auto
[button name="role_button" role="auto" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'auto.png'" enterimg="&tf.sysbtn_img_path + 'auto2.png'" autoimg="&tf.sysbtn_img_path + 'auto4.png'" x="&tf.sysbtn_posx[4]" y="&tf.sysbtn_posy[0]"]

; Skip
[button name="role_button" role="skip" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'skip.png'" enterimg="&tf.sysbtn_img_path + 'skip2.png'" skipimg="&tf.sysbtn_img_path + 'skip4.png'" x="&tf.sysbtn_posx[5]" y="&tf.sysbtn_posy[0]"]

; Backlog
[button name="role_button" role="backlog" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'log.png'" enterimg="&tf.sysbtn_img_path + 'log2.png'" x="&tf.sysbtn_posx[6]" y="&tf.sysbtn_posy[0]"]

; Screen
[button name="role_button" role="fullscreen" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'screen.png'" enterimg="&tf.sysbtn_img_path + 'screen2.png'" x="&tf.sysbtn_posx[7]" y="&tf.sysbtn_posy[0]"]

; Config
[button name="role_button" role="sleepgame" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'sleep.png'" enterimg="&tf.sysbtn_img_path + 'sleep2.png'" x="&tf.sysbtn_posx[8]" y="&tf.sysbtn_posy[0]" storage="../others/plugin/theme_kopanda_20/config.ks"]

; Title
[button name="role_button" role="title" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'title.png'" enterimg="&tf.sysbtn_img_path + 'title2.png'" x="&tf.sysbtn_posx[9]" y="&tf.sysbtn_posy[0]"]

; HideText
[button name="role_button" role="window" width="&tf.sysbtn_img_width" height="&tf.sysbtn_img_height" graphic="&tf.sysbtn_img_path + 'close.png'" enterimg="&tf.sysbtn_img_path + 'close2.png'" x="&tf.sysbtn_posx[10]" y="&tf.sysbtn_posy[0]"]

; Menu
[button name="role_button" role="menu" width="72" height="75" graphic="&tf.sysbtn_img_path + 'menu.png'" enterimg="&tf.sysbtn_img_path + 'menu2.png'" x="&tf.sysbtn_posx[11]" y="&tf.sysbtn_posy[1]"]

[endmacro]

;=======================================================================================================================

; システムで利用するHTML,CSSの設定

;=======================================================================================================================
; セーブ画面
[sysview type="save" storage="./data/others/plugin/theme_kopanda_20/html/save.html"]

; ロード画面
[sysview type="load" storage="./data/others/plugin/theme_kopanda_20/html/load.html"]

; バックログ画面
[sysview type="backlog" storage="./data/others/plugin/theme_kopanda_20/html/backlog.html"]

; メニュー画面
[sysview type="menu" storage="./data/others/plugin/theme_kopanda_20/html/menu.html"]

; CSS
[loadcss file="./data/others/plugin/theme_kopanda_20/ts20.css"]

;=======================================================================================================================

; テストメッセージ出力プラグインの読み込み

;=======================================================================================================================
[loadjs storage="plugin/theme_kopanda_20/testMessagePlus/gMessageTester.js"]
[loadcss file="./data/others/plugin/theme_kopanda_20/testMessagePlus/style.css"]

[macro name="test_message_start"]
[eval exp="gMessageTester.create()"]
[endmacro]

[macro name="test_message_end"]
[eval exp="gMessageTester.destroy()"]
[endmacro]

[macro name="test_message_reset"]
[eval exp="gMessageTester.currentTextNumber=0;gMessageTester.next(true)"]
[endmacro]


[return]
