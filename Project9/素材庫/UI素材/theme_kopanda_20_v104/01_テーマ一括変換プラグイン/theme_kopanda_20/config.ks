; 2023/10/16 @ko10panda edit
;=======================================================================================================================

; コンフィグ画面作成

;=======================================================================================================================
[mask time="100"]

[layopt layer="message0" visible="false"]
[clearfix]
[stop_keyconfig]
[free_layermode time="100" wait="true"]
[reset_camera time="100" wait="true"]
[hidemenubutton]

[iscript]

$(".layer_camera").empty();
$("#bgmovie").remove();

TG.config.autoRecordLabel = "true";

/*
 * Variables Used Only in the Config Mode Screen
 *
 * tf.img_path            共通：画像類のパス
 * tf.uiConfig            コンフィグで使用する画像、サイズ、配置座標を管理するオブジェクト

 * tf.config_num_bgm       BGM音量：インデックス
 * tf.config_num_se        SE音量：インデックス
 * tf.config_num_ch        テキスト速度：インデックス
 * tf.config_num_auto      オートウェイト：インデックス
 *
 * tf.current_bgm_vol     BGM音量：現在のBGM音量
 * tf.current_se_vol      SE音量：現在のSE音量
 * tf.current_ch_speed    テキスト速度：現在のテキスト速度
 * tf.current_auto_speed  オートウェイト：現在のオートウェイト
 *
 * tf.text_skip           未読スキップ：現在の未読スキップの状態
 * tf.screen_size         画面サイズ：現在の画面サイズ
 *
 * sf.prev_vol_list       BGM、SE：BGMとSEの音量とインデックスを保存する配列
 *
*/

// 画像のパス
tf.img_path = '../others/plugin/theme_kopanda_20/image/config/';

tf.uiConfig = {

	img_btn: tf.img_path + 'c_btn.gif',

	gauge : {
		img            : tf.img_path + 'gauge_act.png',
		img_hov        : tf.img_path + 'gauge_hov.png',
		width          : 48,
		height         : 48,
		posx           : [0, 360, 424, 488, 552, 616, 680, 744, 808, 872, 936],
		posy           : [170, 250, 330, 410]
	},

	mute : {
		img_bgm        : tf.img_path + 'mute_bgm_act.png',
		img_bgm_hov    : tf.img_path + 'mute_bgm_hov.png',
		width_bgm      : 140,
		height_bgm     : 50,
		pos_bgm        : [1056, 170],
		img_se         : tf.img_path + 'mute_se_act.png',
		img_se_hov     : tf.img_path + 'mute_se_hov.png',
		width_se       : 124,
		height_se      : 50,
		pos_se         : [1056, 250]
	},

	skip : {
		img_off        : tf.img_path + 'skip_off_act.png',
		img_off_hov    : tf.img_path + 'skip_off_hov.png',
		width_off      : 104,
		height_off     : 48,
		pos_off        : [360, 490],
		img_on         : tf.img_path + 'skip_on_act.png',
		img_on_hov     : tf.img_path + 'skip_on_hov.png',
		width_on       : 96,
		height_on      : 48,
		pos_on         : [488, 490]
	}
};

	// BGM Volume
	const BGM_VOL_VALUES    = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100];
	tf.current_bgm_vol      = parseInt(TG.config.defaultBgmVolume);
	tf.config_num_bgm       = BGM_VOL_VALUES.indexOf(tf.current_bgm_vol);

	// SE Volume
	const SE_VOL_VALUES     = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100];
	tf.current_se_vol       = parseInt(TG.config.defaultSeVolume);
	tf.config_num_se        = SE_VOL_VALUES.indexOf(tf.current_se_vol);

	// Text Speed
	const CH_SPEED_VALUES   = [100, 80, 50, 40, 30, 25, 20, 11, 8, 5];
	tf.current_ch_speed     = parseInt(TG.config.chSpeed);
	tf.config_num_ch        = CH_SPEED_VALUES.indexOf(tf.current_ch_speed);

	// Auto Text Speed
	const AUTO_SPEED_VALUES = [5000, 4500, 4000, 3500, 3000, 2500, 2000, 1300, 800, 500];
	tf.current_auto_speed   = parseInt(TG.config.autoSpeed);
	tf.config_num_auto      = AUTO_SPEED_VALUES.indexOf(tf.current_auto_speed);

	// Skip Text
	tf.text_skip ="ON";
		if(TG.config.unReadTextSkip != "true") {
			tf.text_skip ="OFF";
		}

	// Array variable to store BGM and SE volumes before mute
	sf.prev_vol_list = sf.prev_vol_list ?? [tf.current_bgm_vol, tf.config_num_bgm, tf.current_se_vol, tf.config_num_se];

[endscript]

[cm]

;-----------------------------------------------------------------------------------------------------------------------
; background
;-----------------------------------------------------------------------------------------------------------------------
[image layer="base" x="0" y="0" storage="&tf.img_path +'config_bg.png'" time="100" wait="false"]

;-----------------------------------------------------------------------------------------------------------------------
; back button
;-----------------------------------------------------------------------------------------------------------------------
[button fix="true" graphic="&tf.img_path + 'btn_back.png'" enterimg="&tf.img_path + 'btn_back_hov.png'" target="*backtitle" x="1212" y="24"]

[jump target="*config_page"]

*config_page

[clearstack]
;-----------------------------------------------------------------------------------------------------------------------
; BGM Volume
;-----------------------------------------------------------------------------------------------------------------------
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[1]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  10; tf.config_num_bgm =  1"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[2]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  20; tf.config_num_bgm =  2"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[3]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  30; tf.config_num_bgm =  3"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[4]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  40; tf.config_num_bgm =  4"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[5]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  50; tf.config_num_bgm =  5"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[6]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  60; tf.config_num_bgm =  6"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[7]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  70; tf.config_num_bgm =  7"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[8]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  80; tf.config_num_bgm =  8"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[9]"  y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol =  90; tf.config_num_bgm =  9"]
[button name="bgmvol" fix="true" target="*vol_bgm_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[10]" y="&tf.uiConfig.gauge.posy[0]" exp="tf.current_bgm_vol = 100; tf.config_num_bgm = 10"]

; BGMミュート
[button name="bgmvol" fix="true" target="*vol_bgm_mute" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.mute.img_bgm_hov" width="&tf.uiConfig.mute.width_bgm" height="&tf.uiConfig.mute.height_bgm" x="&tf.uiConfig.mute.pos_bgm[0]" y="&tf.uiConfig.mute.pos_bgm[1]"]

;-----------------------------------------------------------------------------------------------------------------------
; SE Volume
;-----------------------------------------------------------------------------------------------------------------------
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[1]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  10; tf.config_num_se =  1"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[2]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  20; tf.config_num_se =  2"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[3]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  30; tf.config_num_se =  3"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[4]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  40; tf.config_num_se =  4"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[5]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  50; tf.config_num_se =  5"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[6]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  60; tf.config_num_se =  6"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[7]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  70; tf.config_num_se =  7"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[8]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  80; tf.config_num_se =  8"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[9]"  y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol =  90; tf.config_num_se =  9"]
[button name="sevol" fix="true" target="*vol_se_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[10]" y="&tf.uiConfig.gauge.posy[1]" exp="tf.current_se_vol = 100; tf.config_num_se = 10"]

; Mute SE
[button name="sevol" fix="true" target="*vol_se_mute" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.mute.img_se_hov" width="&tf.uiConfig.mute.width_se" height="&tf.uiConfig.mute.height_se" x="&tf.uiConfig.mute.pos_se[0]" y="&tf.uiConfig.mute.pos_se[1]"]

;-----------------------------------------------------------------------------------------------------------------------
; Text Speed
;-----------------------------------------------------------------------------------------------------------------------
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[1]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed =100; tf.config_num_ch = 0"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[2]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 80; tf.config_num_ch = 1"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[3]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 50; tf.config_num_ch = 2"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[4]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 40; tf.config_num_ch = 3"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[5]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 30; tf.config_num_ch = 4"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[6]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 25; tf.config_num_ch = 5"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[7]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 20; tf.config_num_ch = 6"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[8]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed = 11; tf.config_num_ch = 7"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[9]"  y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed =  8; tf.config_num_ch = 8"]
[button name="ch" fix="true" target="*ch_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[10]" y="&tf.uiConfig.gauge.posy[2]" exp="tf.set_ch_speed =  5; tf.config_num_ch = 9"]

;-----------------------------------------------------------------------------------------------------------------------
; Auto Text Speed
;-----------------------------------------------------------------------------------------------------------------------
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[1]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 5000; tf.config_num_auto = 0"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[2]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 4500; tf.config_num_auto = 1"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[3]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 4000; tf.config_num_auto = 2"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[4]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 3500; tf.config_num_auto = 3"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[5]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 3000; tf.config_num_auto = 4"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[6]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 2500; tf.config_num_auto = 5"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[7]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 2000; tf.config_num_auto = 6"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[8]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed = 1300; tf.config_num_auto = 7"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[9]"  y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed =  800; tf.config_num_auto = 8"]
[button name="auto" fix="true" target="*auto_speed_change" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.gauge.img_hov" width="&tf.uiConfig.gauge.width" height="&tf.uiConfig.gauge.height" x="&tf.uiConfig.gauge.posx[10]" y="&tf.uiConfig.gauge.posy[3]" exp="tf.set_auto_speed =  500; tf.config_num_auto = 9"]

;-----------------------------------------------------------------------------------------------------------------------
; Unread Text Skip
;-----------------------------------------------------------------------------------------------------------------------
; Off
[button name="unread_off" fix="true" target="*skip_off" graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.skip.img_off_hov" width="&tf.uiConfig.skip.width_off" height="&tf.uiConfig.skip.height_off" x="&tf.uiConfig.skip.pos_off[0]" y="&tf.uiConfig.skip.pos_off[1]"]

; On
[button name="unread_on"  fix="true" target="*skip_on"  graphic="&tf.uiConfig.img_btn" enterimg="&tf.uiConfig.skip.img_on_hov" width="&tf.uiConfig.skip.width_on" height="&tf.uiConfig.skip.height_on" x="&tf.uiConfig.skip.pos_on[0]" y="&tf.uiConfig.skip.pos_on[1]"]

;-----------------------------------------------------------------------------------------------------------------------
; Load on Config Mode startup
;-----------------------------------------------------------------------------------------------------------------------
[layopt layer="0" visible="true"]

[call target="*load_bgm_img"]
[call target="*load_se_img"]
[call target="*load_ch_img"]
[call target="*load_auto_img"]
[call target="*load_skip_img"]

;test message
[test_message_start]

[mask_off time="300"]

[s]

;-----------------------------------------------------------------------------------------------------------------------
; Exit Config Mode
;-----------------------------------------------------------------------------------------------------------------------
*backtitle
[mask time="250"]

[cm]
[layopt layer="message1" visible="false"]
[endkeyframe]
[freeimage layer="0"]
[freeimage layer="base"]
[clearfix]
[clearstack]
[start_keyconfig]

[mask_off time="10"]

[awakegame]

;=======================================================================================================================

; Handle button click

;=======================================================================================================================
;-----------------------------------------------------------------------------------------------------------------------
; BGM Volume
;-----------------------------------------------------------------------------------------------------------------------
*vol_bgm_mute
[iscript]

if(tf.current_bgm_vol != 0) {
	sf.prev_vol_list[0] = tf.current_bgm_vol;
	sf.prev_vol_list[1] = tf.config_num_bgm;
	tf.current_bgm_vol = 0;
	tf.config_num_bgm  = 0;
} else {
	tf.current_bgm_vol = sf.prev_vol_list[0];
	tf.config_num_bgm  = sf.prev_vol_list[1];
}

[endscript]

*vol_bgm_change
[free layer="0" name="bgmvol" time="0" wait="true"]
[call target="*load_bgm_img"]
[bgmopt volume="&tf.current_bgm_vol"]

[return]

;-----------------------------------------------------------------------------------------------------------------------
; SE Volume
;-----------------------------------------------------------------------------------------------------------------------
*vol_se_mute
[iscript]

if( tf.current_se_vol != 0 ) {
	sf.prev_vol_list[2] = tf.current_se_vol;
	sf.prev_vol_list[3] = tf.config_num_se;
	tf.current_se_vol  = 0;
	tf.config_num_se   = 0;
} else {
	tf.current_se_vol = sf.prev_vol_list[2];
	tf.config_num_se  = sf.prev_vol_list[3];
}

[endscript]

*vol_se_change
[free layer="0" name="sevol" time="0" wait="true"]
[call target="*load_se_img"]
[seopt volume="&tf.current_se_vol"]

[return]

;-----------------------------------------------------------------------------------------------------------------------
; Text Speed
;-----------------------------------------------------------------------------------------------------------------------
*ch_speed_change
[eval exp="tf.current_ch_speed = tf.set_ch_speed"]
[free layer="0" name="ch" time="0" wait="true"]
[call target="*load_ch_img"]
[configdelay speed="&tf.set_ch_speed"]
[test_message_reset]

[return]

;-----------------------------------------------------------------------------------------------------------------------
; Auto Text Speed
;-----------------------------------------------------------------------------------------------------------------------
*auto_speed_change
[eval exp="tf.current_auto_speed = tf.set_auto_speed"]
[free layer="0" name="auto" time="0" wait="true"]
[call target="*load_auto_img"]
[autoconfig speed="&tf.set_auto_speed"]

[return]

;-----------------------------------------------------------------------------------------------------------------------
; Unread Text Skip -- Off
;-----------------------------------------------------------------------------------------------------------------------
*skip_off
[free layer="0" name="unread_on" time="10"]
[image layer="0" name="unread_off" storage="&tf.uiConfig.skip.img_off" x="&tf.uiConfig.skip.pos_off[0]" y="&tf.uiConfig.skip.pos_off[1]"]
[config_record_label skip="false"]

[return]

;-----------------------------------------------------------------------------------------------------------------------
; Unread Text Skip -- On
;-----------------------------------------------------------------------------------------------------------------------
*skip_on
[free layer="0" name="unread_off" time="10"]
[image layer="0" name="unread_on" storage="&tf.uiConfig.skip.img_on" x="&tf.uiConfig.skip.pos_on[0]" y="&tf.uiConfig.skip.pos_on[1]"]
[config_record_label skip="true"]

[return]

;=======================================================================================================================

; Update image

;=======================================================================================================================
;-----------------------------------------------------------------------------------------------------------------------
*load_bgm_img
;-----------------------------------------------------------------------------------------------------------------------
[if exp="tf.config_num_bgm == 0"]
	[image layer="0" name="bgmvol" storage="&tf.uiConfig.mute.img_bgm" x="&tf.uiConfig.mute.pos_bgm[0]" y="&tf.uiConfig.mute.pos_bgm[1]"]
[else]
	[image layer="0" name="bgmvol" storage="&tf.uiConfig.gauge.img" x="&tf.uiConfig.gauge.posx[tf.config_num_bgm]" y="&tf.uiConfig.gauge.posy[0]"]
[endif]

[return]

;-----------------------------------------------------------------------------------------------------------------------
*load_se_img
;-----------------------------------------------------------------------------------------------------------------------
[if exp="tf.config_num_se == 0"]
	[image layer="0" name="sevol" storage="&tf.uiConfig.mute.img_se" x="&tf.uiConfig.mute.pos_se[0]" y="&tf.uiConfig.mute.pos_se[1]"]
[else]
	[image layer="0" name="sevol" storage="&tf.uiConfig.gauge.img" x="&tf.uiConfig.gauge.posx[tf.config_num_se]" y="&tf.uiConfig.gauge.posy[1]"]
[endif]

[return]

;-----------------------------------------------------------------------------------------------------------------------
*load_ch_img
;-----------------------------------------------------------------------------------------------------------------------
[image layer="0" name="ch" storage="&tf.uiConfig.gauge.img" x="&tf.uiConfig.gauge.posx[tf.config_num_ch + 1]" y="&tf.uiConfig.gauge.posy[2]"]

[return]

;-----------------------------------------------------------------------------------------------------------------------
*load_auto_img
;-----------------------------------------------------------------------------------------------------------------------
[image layer="0" name="auto"  storage="&tf.uiConfig.gauge.img" x="&tf.uiConfig.gauge.posx[tf.config_num_auto + 1]" y="&tf.uiConfig.gauge.posy[3]"]

[return]

;-----------------------------------------------------------------------------------------------------------------------
*load_skip_img
;-----------------------------------------------------------------------------------------------------------------------
[if exp="tf.text_skip == 'ON'"]
	[image layer="0" name="unread_on"  storage="&tf.uiConfig.skip.img_on" x="&tf.uiConfig.skip.pos_on[0]" y="&tf.uiConfig.skip.pos_on[1]"]
[else]
	[image layer="0" name="unread_off" storage="&tf.uiConfig.skip.img_off" x="&tf.uiConfig.skip.pos_off[0]" y="&tf.uiConfig.skip.pos_off[1]"]
[endif]

[return]
