using System;
using System.Reflection;
using System.Windows.Forms;
using CSScriptLibrary;

namespace ScriptingInterface
{
	public interface IScript
	{
		string ToIssueDateString(int iYear,int iMonth,int iDay);
	}

	public class ScriptEngine
	{
		private static AsmHelper _AsmHelper = null;
		public ScriptEngine(string sCode)
		{
			try
			{
				Assembly oScript = CSScript.LoadCode(sCode);
				_AsmHelper = new AsmHelper(oScript);
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		public string ToString(int iYear, int iMonth, int iDay)
		{
			var IssueDateHelper = _AsmHelper.GetStaticMethod();
			return IssueDateHelper(iYear, iMonth, iDay).ToString();
		}

        public string ConvertToLineNo(string sLineNo)
        {
            var IssueDateHelper = _AsmHelper.GetStaticMethod();
            return IssueDateHelper(sLineNo).ToString();
        }
	}
}
