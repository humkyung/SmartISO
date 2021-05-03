using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

/* -------------------------------------------------------------------------------
* Late Binding을 통한 Excel Automation class
* 
* *****************  Version 1  *****************
* author: HumKyung Baek Date: 2011.02.28
--------------------------------------------------------------------------------*/
class ExcelAutomation
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="bVisible"></param>
	public ExcelAutomation(bool bVisible)
	{
		excel = Type.GetTypeFromProgID("Excel.Application");
		objExcel = Activator.CreateInstance(excel);
		
		_Visible = bVisible;
		/*
		 * ///objWorkbook = objWorkbooks.GetType().InvokeMember("Add", BindingFlags.InvokeMethod, null, objWorkbooks, null);
		parameter[0] = 1;
		objWorksheet = objWorksheets.GetType().InvokeMember("item", BindingFlags.GetProperty, null, objWorksheets, parameter);
		objRange = objWorksheet.GetType().InvokeMember("UsedRange", BindingFlags.GetProperty, null, objWorksheet, null);
		objRows = objRange.GetType().InvokeMember("Rows", BindingFlags.GetProperty, null, objRange, null);
		object objRowCount = objRows.GetType().InvokeMember("Count", BindingFlags.GetProperty, null, objRows, null);
		objCols = objRange.GetType().InvokeMember("Columns", BindingFlags.GetProperty, null, objRange, null);
		object objColCount = objCols.GetType().InvokeMember("Count", BindingFlags.GetProperty, null, objCols, null);

		parameter[0] = true;
		excel.InvokeMember("Visible", BindingFlags.SetProperty, null, objExcel, parameter);
		 **/
	}

	/// <summary>
	/// descturcture
	/// </summary>
	~ExcelAutomation()
	{
		try
		{
			//! close workbooks and exit application
			objWorkbooks.GetType().InvokeMember("Close", BindingFlags.InvokeMethod, null, objWorkbooks, null);
			System.Runtime.InteropServices.Marshal.ReleaseComObject(objExcel);
		}
		catch(Exception ex)
		{
			Console.WriteLine("Error message : {0}", ex.Message);
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="FilePath"></param>
	/// <returns></returns>
	public bool Open(string FilePath)
	{
		object[] parameter = new object[1];
		parameter[0] = FilePath;
		////objExcel.GetType().InvokeMember("Open", BindingFlags.InvokeMethod, null, objExcel, parameter);
		objWorkbooks = objExcel.GetType().InvokeMember("WorkBooks", BindingFlags.GetProperty, null, objExcel, null);
		_objWorkbook = objWorkbooks.GetType().InvokeMember("Open", BindingFlags.InvokeMethod, null, objWorkbooks, parameter);
		_objWorksheets = _objWorkbook.GetType().InvokeMember("Worksheets", BindingFlags.GetProperty, null, _objWorkbook, null);

		parameter[0] = _Visible;
		excel.InvokeMember("Visible", BindingFlags.SetProperty, null, objExcel, parameter);

		parameter[0] = false;
		this.SetProperty(objExcel, "DisplayAlerts", parameter);

		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sFilePath"></param>
	public void SaveAs(string sFilePath)
	{
		object[] oParameter = new object[1]{sFilePath};
		_objWorkbook.GetType().InvokeMember("SaveAs", BindingFlags.InvokeMethod|BindingFlags.OptionalParamBinding, null, _objWorkbook, oParameter);
	}

	/// <summary>
	/// return active work sheet
	/// </summary>
	/// <returns></returns>
	public object GetActiveSheet()
	{
		return _objWorkbook.GetType().InvokeMember("ActiveSheet", BindingFlags.GetProperty, null, _objWorkbook, null);
	}

	/// <summary>
	/// set active sheet
	/// </summary>
	/// <param name="iSheetNo"></param>
	public object SetActiveSheet(int iSheetNo)
	{
		object objSheet = _objWorksheets.GetType().InvokeMember("item", BindingFlags.GetProperty, null, _objWorksheets, new object[] { iSheetNo });
		objSheet.GetType().InvokeMember("Select", BindingFlags.InvokeMethod , null, objSheet, null);

		return objSheet;
	}

	/// <summary>
	/// set cell value
	/// </summary>
	/// <param name="objWorksheet"></param>
	/// <param name="range"></param>
	/// <param name="value"></param>
	public void SetCellValue(object objWorksheet , object[] range , string value)
	{
		object objRange = objWorksheet.GetType().InvokeMember("Range", BindingFlags.GetProperty, null, objWorksheet, range);
		objRange.GetType().InvokeMember("Value", BindingFlags.SetProperty, null, objRange, new object[] { value });
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="objWorksheet"></param>
	/// <param name="range"></param>
	/// <returns></returns>
	public object[,] GetCellValue(object objWorksheet, object[] range)
	{
		object objRange = GetProperty(objWorksheet, "Range", range);
		object objValue = GetProperty(objRange , "Value", null);
		
		return (object[,])objValue;
	}

	/// <summary>
	/// return string of cell
	/// </summary>
	/// <author>humkyung</author>
	/// <date>2012.09.03</date>
	/// <param name="objWorksheet"></param>
	/// <param name="oCellNo"></param>
	/// <returns></returns>
	public string GetCellValue(object objWorksheet, string sCellNo)
	{
		string res = string.Empty;
		object objRange = GetProperty(objWorksheet, "Range", new object[]{sCellNo,Missing.Value});
		object objValue = GetProperty(objRange, "Value", null);
		res = (null != objValue) ? objValue.ToString() : string.Empty;

		return res;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="objWorksheet"></param>
	/// <param name="range"></param>
	/// <param name="iColorIndex"></param>
	public void SetCellTextColor(object objWorksheet , object[] range , int iColorIndex)
	{
		object objRange = GetProperty(objWorksheet, "Range", range);
		object objFont = GetProperty(objRange, "Font", null);
		SetProperty(objFont, "ColorIndex", new object[] {iColorIndex});
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="objWorksheet"></param>
	/// <param name="oRange"></param>
	/// <param name="iColorIndex"></param>
	public void SetCellTextColor(object objWorksheet, object oRange, int iColorIndex)
	{
		object oFont = GetProperty(oRange, "Font", null);
		SetProperty(oFont, "ColorIndex", new object[] { iColorIndex });
	}

	/// <summary>
	/// get used range of worksheet
	/// </summary>
	/// <author>humkyung</author>
	/// <date>2012.09.03</date>
	/// <param name="objWorksheet"></param>
	/// <returns></returns>
	public int[] GetUsedRange(object objWorksheet)
	{
		int[] res = new int[2];
		object objRange = objWorksheet.GetType().InvokeMember("UsedRange", BindingFlags.GetProperty, null, objWorksheet, null);
		object objRows = objRange.GetType().InvokeMember("Rows", BindingFlags.GetProperty, null, objRange, null);
		res[0] = Convert.ToInt32(objRows.GetType().InvokeMember("Count", BindingFlags.GetProperty, null, objRows, null));
		object objCols = objRange.GetType().InvokeMember("Columns", BindingFlags.GetProperty, null, objRange, null);
		res[1] = Convert.ToInt32(objCols.GetType().InvokeMember("Count", BindingFlags.GetProperty, null, objCols, null));

		return res;
	}

	/// <summary>
	/// make cell no with row and column index
	/// </summary>
	/// <param name="iRowIndex"></param>
	/// <param name="iColumnIndex"></param>
	/// <returns></returns>
	public string MakeCellNo(int iRowIndex, int iColumnIndex)
	{
		string str;
		if (iRowIndex < 1) iRowIndex = 1;
		if (iColumnIndex > 26)
		{
			str = Convert.ToChar('A' + (iColumnIndex - 1) / 26 - 1).ToString() + Convert.ToChar('A' + (iColumnIndex - 1) % 26) + iRowIndex.ToString();
		}
		else
		{
			str = Convert.ToChar('A' + (iColumnIndex - 1) % 26).ToString() + iRowIndex.ToString();
		}

		return str;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="sProperty"></param>
	/// <param name="parameter"></param>
	public void SetProperty(object obj, string sProperty, object[] parameter)
	{
		obj.GetType().InvokeMember(sProperty, BindingFlags.SetProperty, null, obj, parameter);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="sProperty"></param>
	/// <param name="parameter"></param>
	/// <returns></returns>
	public object GetProperty(object obj, string sProperty, object[] parameter)
	{
		return obj.GetType().InvokeMember(sProperty, BindingFlags.GetProperty, null, obj, parameter);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="sProperty"></param>
	/// <param name="parameter"></param>
	/// <returns></returns>
	public object InvokeMethod(object obj, string sProperty, object[] parameter)
	{
		return obj.GetType().InvokeMember(sProperty, BindingFlags.InvokeMethod, null, obj, parameter);
	}

	private Type excel;
	private object objExcel;
	private object objWorkbooks , _objWorkbook;
	public object Workbook
	{
		get
		{
			return _objWorkbook;
		}
	}
	private object _objWorksheets;
	public object Worksheets
	{
		get
		{
			return _objWorksheets;
		}
	}

	private bool _Visible = false;
	public bool Visible
	{
		get
		{
			return _Visible;
		}
		set
		{
			_Visible = value;
		}
	}

	private bool _DisplayAlerts = true;
	public bool DisplayAlerts
	{
		get { return _DisplayAlerts; }
		set 
		{
			_DisplayAlerts = value;

			object[] parameter = new object[1];
			parameter[0] = _DisplayAlerts;
			this.SetProperty(objExcel, "DisplayAlerts", parameter);
		}
	}
}
