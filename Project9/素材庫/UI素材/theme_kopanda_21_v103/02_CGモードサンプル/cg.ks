;=======================================================================================================================
; CGモード画面作成
;=======================================================================================================================
; scenario フォルダ内に cg.ks を配置
; image フォルダ内に cgmode_ts26_HD フォルダを配置
; 必要に応じて枚数やページの増減をおこなってください

;-----------------------------------------------------------------------------------------------------------------------
*start
;-----------------------------------------------------------------------------------------------------------------------
[mask time="100"]

[layopt layer="message0" visible="false"]
[layopt layer="0" visible="true"]
[layopt layer="1" visible="true"]
[hidemenubutton]
[clearfix]

[cm]

[iscript]

	tf.page              = 0; // ページ番号
	tf.selected_cg_image = []; // 選択したCGの差分を格納した配列変数
	tf.cg_index          = 0; // 上の配列の要素番号

	tf.cg_img_path = "../image/cgmode_ts21/"; // 画像のパス

	// 未開放サムネ画像
	tf.cg_thumb = {
		img: tf.cg_img_path + 'lock.png',
		width:  320,
		height: 180,
		posx: [150, 480, 810],
		posy: [210, 400]
	};

	// 前ページボタン
	tf.btn_prev = {
		img:     "cgmode_ts21/btn_prev.png",
		img_hov: "cgmode_ts21/btn_prev_hov.png",
		img_clk: "cgmode_ts21/btn_prev_clk.png",
		width:   48,
		height:  96,
		posx:    48,
		posy:    348
	};

	// 次ページボタン
	tf.btn_next = {
		img:     "cgmode_ts21/btn_next.png",
		img_hov: "cgmode_ts21/btn_next_hov.png",
		img_clk: "cgmode_ts21/btn_next_clk.png",
		width:   48,
		height:  96,
		posx:    1184,
		posy:    348
	};

	// CGモード終了ボタン
	tf.gallery_close = {
		img:     "cgmode_ts21/btn_back.png",
		img_hov: "cgmode_ts21/btn_back_hov.png",
		img_clk: "cgmode_ts21/btn_back_clk.png",
		width:   64,
		height:  64,
		posx:    1192,
		posy:    24
	};

[endscript]

;-----------------------------------------------------------------------------------------------------------------------
; ページネーション（ページ数が変わるときはtextの中身を修正）
;-----------------------------------------------------------------------------------------------------------------------
[macro name="pagination"]
  [layopt layer="0" visible="true"]
  [free layer="0" name="pagination" time="1"]
  [ptext layer="0" name="pagination" text="&tf.page + 1 + '/3'" x="0" y="620" size="18" color="0x443836" width="1280" align="center"]
[endmacro]

;-----------------------------------------------------------------------------------------------------------------------
; 背景
;-----------------------------------------------------------------------------------------------------------------------
[image storage="&tf.cg_img_path +'bg.png'" layer="base" x="0" y="0" time="10" wait="false"]
[image name="label_gallery" storage="&tf.cg_img_path + 'label_gallery.png'" layer="0" width="1280" height="72" x="0" y="30" visible="true" time="10" wait="false"]

[jump target="*cgpage"]

;-----------------------------------------------------------------------------------------------------------------------
*cgpage
;-----------------------------------------------------------------------------------------------------------------------
[cm]

; CGモード終了
[button graphic="&tf.cg_img_path + 'btn_back.png'" enterimg="&tf.cg_img_path + 'btn_back_hov.png'" target="*backtitle" x="&tf.gallery_close.posx" y="&tf.gallery_close.posy"]

; tf.page変数を利用して個別閲覧ボタン作成ラベルにジャンプします
[jump target="& 'page_' + tf.page "]

;-----------------------------------------------------------------------------------------------------------------------
*page_0
;-----------------------------------------------------------------------------------------------------------------------
; 1ページ目

[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[0]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[1]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[2]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]

[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[0]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[1]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[2]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]

; 次ページへ
[button graphic="&tf.btn_next.img" enterimg="&tf.btn_next.img_hov" activeimg="&tf.btn_next.img_clk" target="*nextpage" x="&tf.btn_next.posx" y="&tf.btn_next.posy"]

; ページネーション
[pagination]

; 共通処理にジャンプ
[jump target="*common"]

;-----------------------------------------------------------------------------------------------------------------------
*page_1
;-----------------------------------------------------------------------------------------------------------------------
; 2ページ目

[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[0]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[1]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[2]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]

[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[0]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[1]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[2]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]

; 前のページへ
[button graphic="&tf.btn_prev.img" enterimg="&tf.btn_prev.img_hov" activeimg="&tf.btn_prev.img_clk" target="*backpage" x="&tf.btn_prev.posx" y="&tf.btn_prev.posy"]

; 次のページへ
[button graphic="&tf.btn_next.img" enterimg="&tf.btn_next.img_hov" activeimg="&tf.btn_next.img_clk" target="*nextpage" x="&tf.btn_next.posx" y="&tf.btn_next.posy"]

[pagination]

; 共通処理にジャンプ
[jump target="*common"]

;-----------------------------------------------------------------------------------------------------------------------
*page_2
;-----------------------------------------------------------------------------------------------------------------------
; 3ページ目

[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[0]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[1]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[2]" y="&tf.cg_thumb.posy[0]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]

[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[0]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[1]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]
[cg_image_button graphic="" thumb="" no_graphic="&tf.cg_thumb.img" x="&tf.cg_thumb.posx[2]" y="&tf.cg_thumb.posy[1]" width="&tf.cg_thumb.width" height="&tf.cg_thumb.height"]

; 前のページへ
[button graphic="&tf.btn_prev.img" enterimg="&tf.btn_prev.img_hov" activeimg="&tf.btn_prev.img_clk" target="*backpage" x="&tf.btn_prev.posx" y="&tf.btn_prev.posy"]

[pagination]

; 共通処理にジャンプ
[jump target="*common"]

;-----------------------------------------------------------------------------------------------------------------------
*common
;-----------------------------------------------------------------------------------------------------------------------
[mask_off  time="300"]
[s]

;-----------------------------------------------------------------------------------------------------------------------
; CGモード終了
;-----------------------------------------------------------------------------------------------------------------------
*backtitle
[mask time="200"]

; 使用したレイヤーをすべて消去
[cm]
[freeimage layer="0"]
[freeimage layer="1"]
[freeimage layer="base"]

[mask_off time="200"]

; 別のシナリオにジャンプする場合はここを変更
[jump storage="title.ks"]

;-----------------------------------------------------------------------------------------------------------------------
; 次のページ
;-----------------------------------------------------------------------------------------------------------------------
*nextpage

; 一時変数 tf.page を増加させたうえで *cgpage へ
[eval exp="tf.page++"]
[jump target="*cgpage"]

;-----------------------------------------------------------------------------------------------------------------------
; 前のページ
;-----------------------------------------------------------------------------------------------------------------------
*backpage
; 一時変数 tf.page を減少させたうえで *cgpage へ
[eval exp="tf.page--"]
[jump target="*cgpage"]

;-----------------------------------------------------------------------------------------------------------------------
; 未解放のCGをクリックしたときは何もしない
;-----------------------------------------------------------------------------------------------------------------------
*no_image
[jump target="*cgpage"]

;-----------------------------------------------------------------------------------------------------------------------
; 解放済みのCGをクリックしたときの処理
;-----------------------------------------------------------------------------------------------------------------------
*clickcg

; フリーレイヤーとレイヤー１(back)を解放します
[cm]
[freeimage layer="1" page="back"]

; 一時変数 tf.cg_index に 0 をぶち込みます
[eval exp="tf.cg_index = 0"]

;-----------------------------------------------------------------------------------------------------------------------
; CGの閲覧処理
;-----------------------------------------------------------------------------------------------------------------------
*cg_next_image

; CGを見ていきます。
; 見るべきCGがひとつしかない場合は、それだけ見たあと *cgpage に戻ります。
; 見るべきCGが複数ある場合(差分がある場合)は、
; 再帰的にこのラベルに飛び直して次のCGを見ていきます。

; 一時変数 tf.storage に表示すべきCGのstorageを代入します
[iscript]
tf.storage = tf.selected_cg_image[tf.cg_index];
[endscript]

; CGを表示してクリックを待ちます。
[freeimage layer="1" page="back"]
[image     layer="1" page="back" storage="&tf.storage" folder="bgimage" width="1280" height="720"]
[trans     layer="1" time="700"]
[wt]
[l]

; クリックされたら
; 一時変数 tf.cg_index (差分画像がある場合の画像番号)を1増加させます。
[eval exp="tf.cg_index++"]

; まだ表示すべき差分画像が残っているなら、このラベルに飛びなおします。
[if exp=" tf.selected_cg_image.length > tf.cg_index "]
  [jump target="*cg_next_image"]

[else]
  [freeimage layer="1" page="back"]
  [freeimage layer="1" page="fore" time="700"]
  [jump target=*cgpage]

[endif]
