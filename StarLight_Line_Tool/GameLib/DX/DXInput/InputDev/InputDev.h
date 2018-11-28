/**
* @file InputDev.h
* @brief 入力デバイスの基底クラスのヘッダ
* @author Harutaka-Tsujino
*/

#ifndef INPUT_DEV_H
#define INPUT_DEV_H

#include <windows.h>

#include <dinput.h>

/**
* @brief 入力デバイスの基底クラス
*/
class InputDev
{
public:
	InputDev(HWND hWnd) :m_HWND(hWnd) {};

	virtual ~InputDev()
	{
		m_pDInputDev->Release();
	}

	/**
	* @brief 入力デバイスの状態の取得,その後詳細な振り分けを行う,メッセージループの始まりで呼ぶ
	*/
	virtual VOID UpdataInputState() = 0;

	/**
	* @brief 入力状態の詳細な振り分けを行うため入力状態の保存を行う,メッセージループの終わりで呼ぶ
	*/
	virtual VOID StorePrevInputState() = 0;

	/**
	* @brief 引数のキーが押された瞬間かを返す
	* @param key 判別したいキーの番号,マウスの場合DIM列挙体,キーボードの場合DIK定数
	* @return 押された瞬間の場合TRUE,そうでなければFALSE
	*/
	virtual BOOL IsPressed(INT key) const = 0;

	/**
	* @brief 引数のキーが押されているかを返す
	* @param key 判別したいキーの番号,マウスの場合DIM列挙体,キーボードの場合DIK定数
	* @return 押されている場合TRUE,そうでなければFALSE
	*/
	virtual BOOL IsHeld(INT key) const = 0;

	/**
	* @brief 引数のキーがはなされた瞬間かを返す
	* @param key 判別したいキーの番号,マウスの場合DIM列挙体,キーボードの場合DIK定数
	* @return はなされた瞬間の場合TRUE,そうでなければFALSE
	*/
	virtual BOOL IsReleased(INT key) const = 0;

	/**
	* @brief 引数のキーがはなされているかを返す
	* @param key 判別したいキーの番号,マウスの場合DIM列挙体,キーボードの場合DIK定数
	* @return はなされている場合TRUE,そうでなければFALSE
	*/
	virtual BOOL IsNeutral(INT key) const = 0;

	/**
	* @brief 何か入力されたを返す
	* @return 入力されていたらTRUE
	*/
	virtual BOOL AnyKeyIsPressed() const = 0;

protected:
	enum INPUT_DETAIL
	{
		IND_PRESS,										//押された瞬間のフレームだけTRUE
		IND_HOLD,										//押され続けられている間TRUE
		IND_RELEASE,									//離された瞬間のフレームだけTRUE
		IND_NEUTRAL,									//前回と現在のフレームで押されていない間TRUE
		IND_MAX
	};

	virtual VOID AcquireInputState() = 0;
	virtual VOID CheckInputStateDetatils() = 0;

	inline BOOL IsInputted(const BYTE& rKey) const
	{
		return (rKey & 0x80);
	}

	const HWND m_HWND = nullptr;

	LPDIRECTINPUTDEVICE8 m_pDInputDev = nullptr;
};

#endif // !INPUT_DEV_H
