;-----------------------------------------------------------------------------------------------------------------------
; ティラノスクリプト テーマプラグイン theme_kopanda_21
; 作者:こ・ぱんだ
; https://kopacurve.blog.fc2.com/
;-----------------------------------------------------------------------------------------------------------------------

[iscript]

mp.font_color    = mp.font_color    || "0x443836";
mp.name_color    = mp.name_color    || "0xCCB5A3";
mp.frame_opacity = mp.frame_opacity || "255";
mp.font_color2   = mp.font_color2   || "0x443836";
mp.glyph         = mp.glyph         || "on";

if(TG.config.alreadyReadTextColor != "default") {
	TG.config.alreadyReadTextColor = mp.font_color2;
}

[endscript]

; 名前部分のメッセージレイヤ削除
[free name="chara_name_area" layer="message0"]

; メッセージウィンドウの設定
[position layer="message0" width="1280" height="180" top="540" left="0"]
[position layer="message0" frame="../others/plugin/theme_kopanda_21/image/frame_message.png" margint="40" marginl="140" marginr="140" opacity="&mp.frame_opacity" page="fore"]

; 名前枠の設定
[ptext name="chara_name_area" layer="message0" color="&mp.name_color" size="24" x="120" y="540" width="600" align="left"]
[chara_config ptext="chara_name_area"]

; デフォルトのフォントカラー指定
[font color="&mp.font_color"]
[deffont color="&mp.font_color"]

; クリック待ちグリフの設定（on設定時のみ有効）
[if exp="mp.glyph == 'on'"]
[glyph line="../../../data/others/plugin/theme_kopanda_21/image/system/nextpage.png"]
[endif]

;=======================================================================================================================

; 機能ボタンを表示するマクロ

;=======================================================================================================================

; 機能ボタンを表示したいシーンで[add_theme_button]と記述してください（消去は[clearfix name="role_button"]）
[macro name="add_theme_button"]

; 歯車ボタンは不要なので非表示に
[hidemenubutton]

[iscript]

	tf.sysbtn_img_path = '../others/plugin/theme_kopanda_21/image/button/'; // 画像のパス
	tf.sysbtn_posx     = [102,200,298, 396,494, 592, 690, 788, 886, 984, 1082, 1238]; // 配置するX座標
	tf.sysbtn_posy     = [688, 550]; // 配置するY座標

[endscript]

; Q.Save
[button name="role_button" role="quicksave" graphic="&tf.sysbtn_img_path + 'qsave.png'" enterimg="&tf.sysbtn_img_path + 'qsave2.png'" x="&tf.sysbtn_posx[0]" y="&tf.sysbtn_posy[0]"]

; Q.Load
[button name="role_button" role="quickload" graphic="&tf.sysbtn_img_path + 'qload.png'" enterimg="&tf.sysbtn_img_path + 'qload2.png'" x="&tf.sysbtn_posx[1]" y="&tf.sysbtn_posy[0]"]

; Save
[button name="role_button" role="save" graphic="&tf.sysbtn_img_path + 'save.png'" enterimg="&tf.sysbtn_img_path + 'save2.png'" x="&tf.sysbtn_posx[2]" y="&tf.sysbtn_posy[0]"]

; Load
[button name="role_button" role="load" graphic="&tf.sysbtn_img_path + 'load.png'" enterimg="&tf.sysbtn_img_path + 'load2.png'" x="&tf.sysbtn_posx[3]" y="&tf.sysbtn_posy[0]"]

; Auto
[button name="role_button" role="auto" graphic="&tf.sysbtn_img_path + 'auto.png'" enterimg="&tf.sysbtn_img_path + 'auto2.png'" autoimg="&tf.sysbtn_img_path + 'auto4.png'" x="&tf.sysbtn_posx[4]" y="&tf.sysbtn_posy[0]"]

; Skip
[button name="role_button" role="skip" graphic="&tf.sysbtn_img_path + 'skip.png'" enterimg="&tf.sysbtn_img_path + 'skip2.png'" skipimg="&tf.sysbtn_img_path + 'skip4.png'" x="&tf.sysbtn_posx[5]" y="&tf.sysbtn_posy[0]"]

; Backlog
[button name="role_button" role="backlog" graphic="&tf.sysbtn_img_path + 'log.png'" enterimg="&tf.sysbtn_img_path + 'log2.png'" x="&tf.sysbtn_posx[6]" y="&tf.sysbtn_posy[0]"]

; Screen
[button name="role_button" role="fullscreen" graphic="&tf.sysbtn_img_path + 'screen.png'" enterimg="&tf.sysbtn_img_path + 'screen2.png'" x="&tf.sysbtn_posx[7]" y="&tf.sysbtn_posy[0]"]

; Config
[button name="role_button" role="sleepgame" graphic="&tf.sysbtn_img_path + 'sleep.png'" enterimg="&tf.sysbtn_img_path + 'sleep2.png'" storage="../others/plugin/theme_kopanda_21/config.ks" x="&tf.sysbtn_posx[8]" y="&tf.sysbtn_posy[0]"]

; Menu
[button name="role_button" role="menu" graphic="&tf.sysbtn_img_path + 'menu.png'" enterimg="&tf.sysbtn_img_path + 'menu2.png'" x="&tf.sysbtn_posx[9]" y="&tf.sysbtn_posy[0]"]

; Title
[button name="role_button" role="title" graphic="&tf.sysbtn_img_path + 'title.png'" enterimg="&tf.sysbtn_img_path + 'title2.png'" x="&tf.sysbtn_posx[10]" y="&tf.sysbtn_posy[0]"]

; Close
[button name="role_button" role="window" graphic="&tf.sysbtn_img_path + 'close.png'" enterimg="&tf.sysbtn_img_path + 'close2.png'" x="&tf.sysbtn_posx[11]" y="&tf.sysbtn_posy[1]"]

[endmacro]

;=======================================================================================================================

; システムで利用するHTML,CSSの設定

;=======================================================================================================================
; セーブ画面
[sysview type="save" storage="./data/others/plugin/theme_kopanda_21/html/save.html"]

; ロード画面
[sysview type="load" storage="./data/others/plugin/theme_kopanda_21/html/load.html"]

; バックログ画面
[sysview type="backlog" storage="./data/others/plugin/theme_kopanda_21/html/backlog.html"]

; メニュー画面
[sysview type="menu" storage="./data/others/plugin/theme_kopanda_21/html/menu.html"]

; CSS
[loadcss file="./data/others/plugin/theme_kopanda_21/ts21.css"]

;=======================================================================================================================

; テストメッセージ出力プラグインの読み込み

;=======================================================================================================================
[loadjs storage="plugin/theme_kopanda_21/testMessagePlus/gMessageTester.js"]
[loadcss file="./data/others/plugin/theme_kopanda_21/testMessagePlus/style.css"]

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
