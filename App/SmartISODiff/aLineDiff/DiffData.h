#pragma once

#include <vector>
using namespace std;

namespace diff
{
class CDiffData
{
public:
	CDiffData();
	CDiffData(vector<int> oInitData);
	~CDiffData();

	static void DeleteInstance(CDiffData* p);	/// 2011.11.09 added by humkyung

	int GetLength() const;
private:
	int m_iLength;
public:
	vector<int> m_oData;
	vector<bool> m_oModified;
};
};