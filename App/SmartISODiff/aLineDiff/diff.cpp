#include "StdAfx.h"
#include <assert.h>
#include <IsString.h>
#include <Tokenizer.h>
#include "diff.h"

using namespace diff;

/// <summary>
/// Find the difference in 2 text documents, comparing by textlines.
/// The algorithm itself is comparing 2 arrays of numbers so when comparing 2 text documents
/// each line is converted into a (hash) number. This hash-value is computed by storing all
/// textlines into a common hashtable so i can find dublicates in there, and generating a 
/// new number each time a new textline is inserted.
/// </summary>
/// <param name="TextA">A-version of the text (usualy the old one)</param>
/// <param name="TextB">B-version of the text (usualy the new one)</param>
/// <param name="trimSpace">When set to true, all leading and trailing whitespace characters are stripped out before the comparation is done.</param>
/// <param name="ignoreSpace">When set to true, all whitespace characters are converted to a single space character before the comparation is done.</param>
/// <param name="ignoreCase">When set to true, all characters are converted to their lowercase equivivalence before the comparation is done.</param>
/// <returns>Returns a array of Items that describe the differences.</returns>
vector<CDiff::Item*> CDiff::DiffText(STRING_T TextA, STRING_T TextB, bool trimSpace, bool ignoreSpace, bool ignoreCase)
{
	/// prepare the input-text and convert to comparable numbers.
	MAP_T<STRING_T,int>* h = new MAP_T<STRING_T,int>;

	// The A-Version of the data (original data) to be compared.
	CDiffData* pDataA = new CDiffData(DiffCodes(TextA, h, trimSpace, ignoreSpace, ignoreCase));

	// The B-Version of the data (modified data) to be compared.
	CDiffData* pDataB = new CDiffData(DiffCodes(TextB, h, trimSpace, ignoreSpace, ignoreCase));

	SAFE_DELETE(h); // free up hashtable memory (maybe)

	const int MAX = pDataA->GetLength() + pDataB->GetLength() + 1;
	/// vector for the (0,0) to (x,y) search
	vector<int> oDownVector(2 * MAX + 2);
	/// vector for the (u,v) to (N,M) search
	vector<int> oUpVector(2 * MAX + 2);

	LCS(pDataA, 0, pDataA->GetLength() , pDataB , 0, pDataB->GetLength() , oDownVector, oUpVector);

	Optimize(pDataA);
	Optimize(pDataB);

	vector<Item*> oResult;
	CreateDiffs(oResult , pDataA , pDataB);

	SAFE_DELETE(pDataA);
	SAFE_DELETE(pDataB);

	return oResult;
}

void CDiff::Optimize(CDiffData* Data) 
{
	int StartPos, EndPos;

	StartPos = 0;
	while (StartPos < Data->GetLength()) 
	{
		while ((StartPos < Data->GetLength()) && (Data->m_oModified[StartPos] == false))
			StartPos++;
		EndPos = StartPos;
		while ((EndPos < Data->GetLength()) && (Data->m_oModified[EndPos] == true))
			EndPos++;

		if ((EndPos < Data->GetLength()) && (Data->m_oData[StartPos] == Data->m_oData[EndPos])) 
		{
			Data->m_oModified[StartPos] = false;
			Data->m_oModified[EndPos] = true;
		}
		else 
		{
			StartPos = EndPos;
		} // if
	} // while
} // Optimize

/// <summary>
/// Find the difference in 2 arrays of integers.
/// </summary>
/// <param name="ArrayA">A-version of the numbers (usualy the old one)</param>
/// <param name="ArrayB">B-version of the numbers (usualy the new one)</param>
/// <returns>Returns a array of Items that describe the differences.</returns>
int CDiff::DiffInt(vector<Item*> oResult , vector<int> ArrayA, vector<int> ArrayB) 
{
	// The A-Version of the data (original data) to be compared.
	CDiffData* DataA = new CDiffData(ArrayA);

	// The B-Version of the data (modified data) to be compared.
	CDiffData* DataB = new CDiffData(ArrayB);

	int MAX = DataA->GetLength() + DataB->GetLength() + 1;
	/// vector for the (0,0) to (x,y) search
	vector<int> oDownVector(2 * MAX + 2);
	/// vector for the (u,v) to (N,M) search
	vector<int> oUpVector(2 * MAX + 2);

	LCS(DataA, 0, DataA->GetLength() , DataB, 0, DataB->GetLength() , oDownVector, oUpVector);

	return CreateDiffs(oResult , DataA, DataB);
} // Diff

/// <summary>
/// This function converts all textlines of the text into unique numbers for every unique textline
/// so further work can work only with simple numbers.
/// </summary>
/// <param name="aText">the input text</param>
/// <param name="h">This extern initialized hashtable is used for storing all ever used textlines.</param>
/// <param name="trimSpace">ignore leading and trailing space characters</param>
/// <returns>a array of integers.</returns>
vector<int> CDiff::DiffCodes(STRING_T aText, MAP_T<STRING_T , int>* h, bool trimSpace, bool ignoreSpace, bool ignoreCase) 
{
	assert(h && "h is NULL");

	if(h)
	{
		// get all codes of the text
		int lastUsedCode = h->size();

		// strip off all cr, only use lf as textline separator.
		IsString::ReplaceOf(aText , _T("\r") , _T(""));
		vector<STRING_T> Lines;
		CTokenizer<CIsFromString>::Tokenize(Lines , aText , CIsFromString(_T("\n")));

		vector<int> Codes(Lines.size());
		for (int i = 0; i < int(Lines.size()); ++i) 
		{
			STRING_T s = Lines[i];
			if (trimSpace) IsString::TrimWhiteSpace(s);

			//if (ignoreSpace)
			//{
			//	s = Regex.Replace(s, "\\s+", " ");            // TODO: optimization: faster blank removal.
			//}

			if (ignoreCase) IsString::ToLower(s);

			MAP_T<STRING_T , int>::iterator where = h->find( s );
			if (h->end() == where) 
			{
				lastUsedCode++;
				h->insert(make_pair(s , lastUsedCode));
				Codes[i] = lastUsedCode;
			}
			else 
			{
				Codes[i] = where->second;
			} // if
		} // for

		return (Codes);
	}

	vector<int> temp;
	return temp;
} // DiffCodes

/// <summary>
/// This is the algorithm to find the Shortest Middle Snake (SMS).
/// </summary>
/// <param name="DataA">sequence A</param>
/// <param name="LowerA">lower bound of the actual range in DataA</param>
/// <param name="UpperA">upper bound of the actual range in DataA (exclusive)</param>
/// <param name="DataB">sequence B</param>
/// <param name="LowerB">lower bound of the actual range in DataB</param>
/// <param name="UpperB">upper bound of the actual range in DataB (exclusive)</param>
/// <param name="DownVector">a vector for the (0,0) to (x,y) search. Passed as a parameter for speed reasons.</param>
/// <param name="UpVector">a vector for the (u,v) to (N,M) search. Passed as a parameter for speed reasons.</param>
/// <returns>a MiddleSnakeData record containing x,y and u,v</returns>
CDiff::SMSRD CDiff::SMS(CDiffData* DataA, int LowerA, int UpperA, CDiffData* DataB, int LowerB, int UpperB, vector<int>& oDownVector, vector<int>& oUpVector) 
{
	SMSRD ret;

	int MAX = DataA->GetLength() + DataB->GetLength() + 1;

	int DownK = LowerA - LowerB; // the k-line to start the forward search
	int UpK = UpperA - UpperB; // the k-line to start the reverse search

	int Delta = (UpperA - LowerA) - (UpperB - LowerB);
	bool oddDelta = (Delta & 1) != 0;

	// The vectors in the publication accepts negative indexes. the vectors implemented here are 0-based
	// and are access using a specific offset: UpOffset UpVector and DownOffset for DownVektor
	int DownOffset = MAX - DownK;
	int UpOffset = MAX - UpK;

	int MaxD = ((UpperA - LowerA + UpperB - LowerB) / 2) + 1;

	// Debug.Write(2, "SMS", String.Format("Search the box: A[{0}-{1}] to B[{2}-{3}]", LowerA, UpperA, LowerB, UpperB));

	// init vectors
	oDownVector[DownOffset + DownK + 1] = LowerA;
	oUpVector[UpOffset + UpK - 1] = UpperA;
	for (int D = 0; D <= MaxD; D++) 
	{
		// Extend the forward path.
		for (int k = DownK - D; k <= DownK + D; k += 2) 
		{
			// Debug.Write(0, "SMS", "extend forward path " + k.ToString());

			// find the only or better starting point
			int x, y;
			if (k == DownK - D) 
			{
				x = oDownVector[DownOffset + k + 1]; // down
			}
			else 
			{
				x = oDownVector[DownOffset + k - 1] + 1; // a step to the right
				if ((k < DownK + D) && (oDownVector[DownOffset + k + 1] >= x))
					x = oDownVector[DownOffset + k + 1]; // down
			}
			y = x - k;

			// find the end of the furthest reaching forward D-path in diagonal k.
			while ((x < UpperA) && (y < UpperB) && (DataA->m_oData[x] == DataB->m_oData[y])) 
			{
				x++; y++;
			}
			oDownVector[DownOffset + k] = x;

			// overlap ?
			if (oddDelta && (UpK - D < k) && (k < UpK + D)) 
			{
				if (oUpVector[UpOffset + k] <= oDownVector[DownOffset + k]) 
				{
					ret.x = oDownVector[DownOffset + k];
					ret.y = oDownVector[DownOffset + k] - k;
					// ret.u = UpVector[UpOffset + k];      // 2002.09.20: no need for 2 points 
					// ret.v = UpVector[UpOffset + k] - k;
					return (ret);
				} // if
			} // if

		} // for k

		// Extend the reverse path.
		for (int k = UpK - D; k <= UpK + D; k += 2) 
		{
			// Debug.Write(0, "SMS", "extend reverse path " + k.ToString());

			// find the only or better starting point
			int x, y;
			if (k == UpK + D) 
			{
				x = oUpVector[UpOffset + k - 1]; // up
			}
			else 
			{
				x = oUpVector[UpOffset + k + 1] - 1; // left
				if ((k > UpK - D) && (oUpVector[UpOffset + k - 1] < x))
					x = oUpVector[UpOffset + k - 1]; // up
			} // if
			y = x - k;

			while ((x > LowerA) && (y > LowerB) && (DataA->m_oData[x - 1] == DataB->m_oData[y - 1])) 
			{
				x--; y--; // diagonal
			}
			oUpVector[UpOffset + k] = x;

			// overlap ?
			if (!oddDelta && (DownK - D <= k) && (k <= DownK + D)) 
			{
				if (oUpVector[UpOffset + k] <= oDownVector[DownOffset + k]) 
				{
					ret.x = oDownVector[DownOffset + k];
					ret.y = oDownVector[DownOffset + k] - k;
					// ret.u = UpVector[UpOffset + k];     // 2002.09.20: no need for 2 points 
					// ret.v = UpVector[UpOffset + k] - k;
					return (ret);
				} // if
			} // if

		} // for k

	} // for D

	throw exception("the algorithm should never come here.");
} // SMS

/// <summary>
/// This is the divide-and-conquer implementation of the longes common-subsequence (LCS) 
/// algorithm.
/// The published algorithm passes recursively parts of the A and B sequences.
/// To avoid copying these arrays the lower and upper bounds are passed while the sequences stay constant.
/// </summary>
/// <param name="DataA">sequence A</param>
/// <param name="LowerA">lower bound of the actual range in DataA</param>
/// <param name="UpperA">upper bound of the actual range in DataA (exclusive)</param>
/// <param name="DataB">sequence B</param>
/// <param name="LowerB">lower bound of the actual range in DataB</param>
/// <param name="UpperB">upper bound of the actual range in DataB (exclusive)</param>
/// <param name="DownVector">a vector for the (0,0) to (x,y) search. Passed as a parameter for speed reasons.</param>
/// <param name="UpVector">a vector for the (u,v) to (N,M) search. Passed as a parameter for speed reasons.</param>
void CDiff::LCS(CDiffData* DataA, int LowerA, int UpperA, CDiffData* DataB, int LowerB, int UpperB, vector<int>& oDownVector, vector<int>& oUpVector) 
{
	// Debug.Write(2, "LCS", String.Format("Analyse the box: A[{0}-{1}] to B[{2}-{3}]", LowerA, UpperA, LowerB, UpperB));

	// Fast walkthrough equal lines at the start
	while (LowerA < UpperA && LowerB < UpperB && DataA->m_oData[LowerA] == DataB->m_oData[LowerB]) 
	{
		LowerA++; LowerB++;
	}

	// Fast walkthrough equal lines at the end
	while (LowerA < UpperA && LowerB < UpperB && DataA->m_oData[UpperA - 1] == DataB->m_oData[UpperB - 1]) 
	{
		--UpperA; --UpperB;
	}

	if (LowerA == UpperA) 
	{
		// mark as inserted lines.
		while (LowerB < UpperB)
			DataB->m_oModified[LowerB++] = true;

	}
	else if (LowerB == UpperB) 
	{
		// mark as deleted lines.
		while (LowerA < UpperA)
			DataA->m_oModified[LowerA++] = true;

	}
	else
	{
		// Find the middle snakea and length of an optimal path for A and B
		SMSRD smsrd = SMS(DataA, LowerA, UpperA, DataB, LowerB, UpperB, oDownVector, oUpVector);
		// Debug.Write(2, "MiddleSnakeData", String.Format("{0},{1}", smsrd.x, smsrd.y));

		// The path is from LowerX to (x,y) and (x,y) to UpperX
		LCS(DataA, LowerA, smsrd.x, DataB, LowerB, smsrd.y, oDownVector, oUpVector);
		LCS(DataA, smsrd.x, UpperA, DataB, smsrd.y, UpperB, oDownVector, oUpVector);  // 2002.09.20: no need for 2 points 
	}
} // LCS()

/// <summary>Scan the tables of which lines are inserted and deleted,
/// producing an edit script in forward order.  
/// </summary>
/// dynamic array
int CDiff::CreateDiffs(vector<Item*>& oResult , CDiffData* DataA, CDiffData* DataB) 
{
	int StartA, StartB;
	int LineA, LineB;

	LineA = 0;
	LineB = 0;
	while (LineA < DataA->GetLength() || LineB < DataB->GetLength()) 
	{
		if ((LineA < DataA->GetLength()) && (!DataA->m_oModified[LineA])
			&& (LineB < DataB->GetLength()) && (!DataB->m_oModified[LineB])) 
		{
				// equal lines
				LineA++;
				LineB++;
		}
		else 
		{
			// maybe deleted and/or inserted lines
			StartA = LineA;
			StartB = LineB;

			while (LineA < DataA->GetLength() && (LineB >= DataB->GetLength() || DataA->m_oModified[LineA]))
				// while (LineA < DataA.Length && DataA.modified[LineA])
				LineA++;

			while (LineB < DataB->GetLength() && (LineA >= DataA->GetLength() || DataB->m_oModified[LineB]))
				// while (LineB < DataB.Length && DataB.modified[LineB])
				LineB++;

			if ((StartA < LineA) || (StartB < LineB)) 
			{
				// store a new difference-item
				CDiff::Item* aItem = new CDiff::Item();
				aItem->StartA = StartA;
				aItem->StartB = StartB;
				aItem->deletedA = LineA - StartA;
				aItem->insertedB = LineB - StartB;
				oResult.push_back(aItem);
			} // if
		} // if
	} // while

	return oResult.size();
}
