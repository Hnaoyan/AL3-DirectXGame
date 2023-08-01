#pragma once
#include <variant>
#include <map>
#include <string>
#include "Vector3.h"

class GlobalVariables 
{
public:
	static GlobalVariables* GetInstance();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// 値のセット(int)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	
	/// <summary>
	/// 値のセット(float)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット(Vector3)
	/// </summary>
	/// <param name="groupName"></param>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);


private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& variable) = default;
	GlobalVariables& operator=(const GlobalVariables& variable);

private:
	// 項目
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector3> value;
	};
	// グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

};
