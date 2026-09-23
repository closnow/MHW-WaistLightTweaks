# MHW Waist Light & Scoutfly Cage Tweaks (LightControl)

[English](#english) | [日本語](#japanese)

---

<a name="english"></a>
## English

A native C++ plugin for *Monster Hunter: World* that finely adjusts and toggles the brightness and range of the Scoutfly cage, guiding Scoutflies, and waist lights.

### ⚠️ Disclaimer & Notice
- **This mod is provided "AS-IS". No technical support, bug fixes, or feature requests will be accepted.**
- Most of the code was created and modified with the **assistance of AI**. As such, the author cannot address technical issues such as code optimization, unexpected bugs, or conflicts with other mods.

### Features
* **Individual Lighting Controls**: Independently adjust the guiding Scoutflies, the waist cage itself, and the forward-facing waist spotlight.
* **Separated Field & Base Profiles**: Configure different brightness/ranges for when you are out in the field vs. in town/hubs.
* **Hotkey Toggle**: Toggle the lights ON/OFF in real-time (Default: **F9** / `0x78`).
* **Live INI Reloading**: Automatically reloads configuration when `WaistLightTweaks.ini` is saved—no need to restart the game
* **Auto-generated INI**: If `WaistLightTweaks.ini` does not exist, it will be automatically created on startup with default values.

### Requirements
* [Stracker's Loader](https://www.nexusmods.com/monsterhunterworld/mods/1982)

### Installation
1. Build the project in **Release x64** to generate `WaistLightTweaks.dll`.
2. Place `WaistLightTweaks.dll` into:  
   `nativePC/plugins/`
3. Launch the game. `WaistLightTweaks.ini` will be automatically generated in the same folder.
4. Edit `WaistLightTweaks.ini` to customize values and hotkey as needed.

---

### INI Configuration Guide (`WaistLightTweaks.ini`)

```ini
[Hotkey]
toggle_key=0x78 ; Virtual-Key Code in hex (0x78 = F9, 0x7B = F12, etc.)

[GuideScoutfly]
brightness=60.0 ; Brightness of the guiding scoutfly swarm flying ahead. Set to 0.0 to completely dim them

[WaistCage] ; Field: Scoutfly cage on your waist
brightness=1000.0 ; Ambient glow brightness of the cage
range=70.0        ; Glow radius

[WaistLight] ; Field: Forward light from your waist
brightness=1500.0 ; Brightness of the spotlight illuminating the path ahead
range=3000.0      ; Distance/range of the beam

[BaseCage] ; Hub / Town: Scoutfly cage on your waist
brightness=1.5
range=70.0

[BaseWaist] ; Hub / Town: Forward light
brightness=54.6
range=800.0
```

> **Note regarding Your Room (Private Suite):**  
> The cage and waist lights inside "Your Room" (My House) are governed by a different internal game mechanic and are not affected by this mod.

---

### ⚠️ Known Issue (Vanilla Game Bug)
After eating a meal at the Canteen or Campsite, the light toggle will consistently stop responding and the light will stay off (100% reproducible).  
This is an inherent bug in the vanilla game itself, NOT caused by this mod.

**How to fix / Workarounds:**
* Sit down on a chair/bench again and stand up.
* Talk to the Handler or any major NPC.
* Move to another location that triggers a loading screen (e.g., returning to town or traveling to a different map. *Note: Fast traveling between camps within the same field does NOT trigger a loading screen and will not fix this*).

### Credits
* Developed for Monster Hunter: World.

---

<a name="japanese"></a>
## 日本語

モンスターハンター：ワールド (MHW) の「導蟲（誘導中の光）」「腰の導蟲ケージ」「前方を照らすライト」の明るさや照射範囲を細かくカスタマイズし、ワンキーでON/OFFできるネイティブC++プラグインです。

### ⚠️ 免責事項・注意事項
* 本MODは「現状渡し (As-is)」となります。カスタマーサポート、要望受付、バグ修正などは一切行いません。
* 本コードの作成・改修の大部分はAIの支援を受けて作成しています。そのため、コードの最適化や予期せぬ不具合、他MODとの競合などの技術的な問題への対応は一切できません。

### 主な機能
* **光ごとの個別カスタマイズ**: 前方を飛んで案内する導蟲、腰のケージ自体の発光、前方を照らすライトをそれぞれ独立して調整可能。
* **フィールドと拠点の個別設定**: クエスト中のフィールドと、アステラ・セリエナなどの拠点で異なる明るさ・範囲を設定可能。
* **ワンキートグル**: 設定したキー（デフォルト: F9）でライトの点灯／消灯をリアルタイムに切り替え。
* **INIホットリロード**: ゲーム起動中に `WaistLightTweaks.ini` を編集して保存すると、ゲームを再起動することなく即座に設定値が反映されます。
* **INI自動生成**: 設定ファイルが存在しない場合、初回起動時にデフォルト設定で `WaistLightTweaks.ini` が自動生成されます。

### 必須環境
* [Stracker's Loader](https://www.nexusmods.com/monsterhunterworld/mods/1982)

### 導入方法
1. Release x64 でビルドして `WaistLightTweaks.dll` を生成します。
2. 生成された `WaistLightTweaks.dll` を以下のフォルダに配置します：  
   `nativePC/plugins/`
3. ゲームを起動すると、同じフォルダ内に `WaistLightTweaks.ini` が自動生成されます。
4. キー設定（toggle_key）や各数値を自由に調整してください。

---

### 設定ファイル解説 (`WaistLightTweaks.ini`)

```ini
[Hotkey]
toggle_key=0x78 ; トグルキーの仮想キーコード (16進数。0x78 = F9, 0x7B = F12 など)

[GuideScoutfly]
brightness=60.0 ; 前方を飛んで案内する導蟲の明るさ (0.0にすると完全に光を消せます)

[WaistCage] ; 【フィールド用】腰に下げた導蟲ケージ
brightness=1000.0 ; ケージ自体のぼんやりした光の明るさ
range=70.0        ; 光の届く範囲

[WaistLight] ; 【フィールド用】ケージから前方を照らすライト
brightness=1500.0 ; 前方を照らすライトの明るさ
range=3000.0      ; 前方を照らす照射距離・範囲

[BaseCage] ; 【拠点用】腰に下げた導蟲ケージ
brightness=1.5
range=70.0

[BaseWaist] ; 【拠点用】前方ライト
brightness=54.6
range=800.0
```

> **マイハウス内の光について:**  
> マイハウス（マイルーム）内のケージおよび腰ライトはゲーム本編の処理が異なるため、本MODの設定は適用されません（仕様となります）。

---

### ⚠️ 既知の不具合（バニラのバグ）
食事場やキャンプで食事をした後、確定でライトのON/OFFが効かなくなり消灯したままになります（100%発生します）。  
これは本MODの不具合ではなく、ゲーム本編（バニラ）由来のバグです。

**復旧方法（回避策）：**
* もう一度椅子に座って立ち上がる
* 受付嬢などの主要NPCに話しかける
* 拠点への帰還や別マップへの移動など、ロード画面を挟む移動を行う（※同フィールド内のキャンプ間ファストトラベルはロードを挟まないため治りません）

### クレジット
* Developed for Monster Hunter: World.
