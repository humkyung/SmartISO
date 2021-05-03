using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;

namespace framework.autocad
{
	public class AcExDbDBObject
	{
		private AcExDbDBObject()
		{
		}

		public static string GetXDataAt(DBObject dbobj, string regApp, int index)
		{
			string str = string.Empty;
			using (ResultBuffer xdataForApplication = dbobj.GetXDataForApplication(regApp))
			{
				if (null != xdataForApplication)
				{
					TypedValue[] typedValueArray = xdataForApplication.AsArray();
					if (typedValueArray.Length > index)
					{
						str = ((TypedValue)@typedValueArray[index]).Value.ToString();
					}
				}
			}
			return str;
		}

		public static List<string> GetXDataAsStringList(DBObject dbobj, string regApp)
		{
			List<string> list = null;
			using (ResultBuffer xdataForApplication = dbobj.GetXDataForApplication(regApp))
			{
				if(null != xdataForApplication)
				{
					TypedValue[] typedValueArray = xdataForApplication.AsArray();
					if (null != typedValueArray)
					{
						list = new List<string>();
						for(int index=0;index < typedValueArray.Length;++index)
						{
							TypedValue typedValue = typedValueArray[index];
							list.Add(typedValue.Value.ToString());
						}
					}
				}
			}
			return list;
		}

		public static string GetXDataAt(DBObject dbobj, string regApp, string key)
		{
			List<string> xdataAsStringList = AcExDbDBObject.GetXDataAsStringList(dbobj, regApp);
			if (xdataAsStringList != null)
			{
				if (xdataAsStringList.Count <= 0) return (string.Empty);
				foreach (string str in xdataAsStringList)
				{
					string[] strArray = str.Split("=".ToCharArray(), 2);
					if (strArray != null && strArray.Length >= 2 && strArray[0] == key)
						return strArray[1];
				}
			}

			return string.Empty;
		}

		/// <summary>
		/// get xdata as dictionary
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="dbobj"></param>
		/// <param name="regApp"></param>
		/// <returns></returns>
		public static Dictionary<string, string> GetXDataAsDictionary(DBObject dbobj, string regApp)
		{
			List<string> xdataAsStringList = AcExDbDBObject.GetXDataAsStringList(dbobj, regApp);
			if (xdataAsStringList == null) return (Dictionary<string, string>) null;
			if(xdataAsStringList.Count > 0)
			{
				Dictionary<string, string> dictionary = new Dictionary<string, string>();
				using(List<string>.Enumerator enumerator = xdataAsStringList.GetEnumerator())
				{
					try
					{
						while (enumerator.MoveNext())
						{
							string[] strArray = enumerator.Current.Split("=".ToCharArray(), 2);
							if ((null != strArray) && (2 == strArray.Length))
							{
								string key = strArray[0];
								string str = strArray[1];
								if (!dictionary.ContainsKey(key))
								{
									dictionary.Add(key, str);
								}
								else
								{
									dictionary[key] = str;
								}
							}
						}
					}
					catch { }
				}
				return dictionary;
			}
			return (Dictionary<string, string>) null;
		}

		public static void SetXDataWithStringList(DBObject dbobj, string regApp, List<string> strlst)
		{
			Database database = dbobj.Database;
			using (Transaction transaction = database.TransactionManager.StartTransaction())
			{
				DBObject @object = transaction.GetObject(dbobj.ObjectId, OpenMode.ForRead);
				List<TypedValue> list = new List<TypedValue>();
				list.Add(new TypedValue(1001, (object) regApp));
				if (null != strlst)
				{
					foreach (string str in strlst)
						list.Add(new TypedValue(1000, (object) str));
				}
				using (ResultBuffer resultBuffer = new ResultBuffer(list.ToArray()))
				{
					@object.UpgradeOpen();
					@object.XData = resultBuffer;
				}
				transaction.Commit();
			}
		}

		public static void SetXDataWithDictionary(DBObject dbobj, string regApp, Dictionary<string, string> dic)
		{
			List<string> strlst = new List<string>();
			using (Dictionary<string, string>.Enumerator enumerator = dic.GetEnumerator())
			{
				while (enumerator.MoveNext())
				{
					KeyValuePair<string, string> current = enumerator.Current;
					strlst.Add(string.Format("{0}", /*(object) current.Key, */current.Value));
				}
			}
			AcExDbDBObject.SetXDataWithStringList(dbobj, regApp, strlst);
		}
	}
}