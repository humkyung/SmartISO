using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.IO;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using acApp = Autodesk.AutoCAD.ApplicationServices.Application;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;
using System.Net.Sockets;
using framework.autocad;
using Autodesk.AutoCAD.Geometry;

namespace SmartISO.NET
{
	public class CADPluginOpenDwgCommand : ICADPluginCommand
	{
		public DataSet Execute(DataSet dsParam)
		{
			DataSet res = new DataSet();
			System.Data.DataTable dtParam = new System.Data.DataTable("Result");
			{
				dtParam.Columns.Add(new System.Data.DataColumn("Value", typeof(bool)));
			}
			res.Tables.Add(dtParam);

			try
			{
				if (dsParam.Tables.Contains("Params") && (1 == dsParam.Tables["Params"].Rows.Count))
				{
					string sFilePath = dsParam.Tables["Params"].Rows[0]["FilePath"].ToString();
                    acApp.DocumentManager.Open(sFilePath);
				}

				DataRow drParam = res.Tables["Result"].NewRow();
				{
					drParam["Value"] = true;
				}
				res.Tables["Result"].Rows.Add(drParam);
			}
			catch { }

			return res;
		}
    }
}
