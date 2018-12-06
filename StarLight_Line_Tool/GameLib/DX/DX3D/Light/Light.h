/**
* @file Light.h
* @brief ライトクラスのヘッダ
* @author Harutaka Tsujino
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <windows.h>

#include <d3dx9.h>

/**
* @brief ライトクラス
*/
class Light
{
public:
	Light(const LPDIRECT3DDEVICE9& rpDX3DDev) :m_rpDX3D_DEV(rpDX3DDev) {};
	~Light() {};

	/**
	* @brief 引数のライト構造体のデータを用いてライトを作成しそれを使う
	* @param rLight ライト構造体 phi等にも値を入れる必要があるときがある
	* @param index 作成するライトに振り分ける識別番号
	*/
	inline VOID SetLight(const D3DLIGHT9& rLight, DWORD index) const
	{
		m_rpDX3D_DEV->SetLight(index, &rLight);
		m_rpDX3D_DEV->LightEnable(index, TRUE);
	}

	/**
	* @brief SetLightで使用しているライトをonにする,SetLightをしたときに自動で呼ばれる
	* @param onにするライトに振り分けられた識別番号
	*/
	inline VOID OnLight(DWORD index) const
	{
		m_rpDX3D_DEV->LightEnable(index, TRUE);
	}

	/**
	* @brief SetLightで使用しているライトをoffにする
	* @param offにするライトに振り分けられた識別番号
	*/
	inline VOID OffLight(DWORD index) const
	{
		m_rpDX3D_DEV->LightEnable(index, FALSE);
	}

	/**
	* @brief ライティングを有効にする
	*/
	inline VOID EnableLighting() const
	{
		m_rpDX3D_DEV->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	/**
	* @brief ライティングを無効にする ライトをすべて無効にする
	*/
	inline VOID DisableLighting() const
	{
		m_rpDX3D_DEV->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	/**
	* @brief 環境光の強さを変更する 呼ばれていない場合環境光は使われない
	* @param aRGB 環境光の強さ 明るい色のほうが強くなる
	*/
	inline VOID ChangeAmbientIntensity(DWORD aRGB) const
	{
		m_rpDX3D_DEV->SetRenderState(D3DRS_AMBIENT, aRGB);
	}

	/**
	* @brief 反射光を有効にする
	*/
	inline VOID EnableSpecular() const
	{
		m_rpDX3D_DEV->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	}

	/**
	* @brief 反射光を無効にする
	*/
	inline VOID DisaableSpecular() const
	{
		m_rpDX3D_DEV->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	}

	/**
	* @brief デフォルトのライティングを使用する ウィンドウモードを切り替えた時には再設定する必要がある
	*/
	VOID DefaultLighting() const
	{
		EnableLighting();
		EnableSpecular();
	}

private:
	const LPDIRECT3DDEVICE9& m_rpDX3D_DEV = nullptr;
};

#endif // !LIGHT_H
