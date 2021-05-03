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

[assembly: CommandClass(typeof(SmartISO.NET.CADPluginPickPointCommand))]
namespace SmartISO.NET
{
    public class CADPluginPickPointCommand : ICADPluginCommand
	{
		public DataSet Execute(DataSet dsParam)
		{
			DataSet res = new DataSet();
			System.Data.DataTable dtParam = new System.Data.DataTable("Result");
			{
				dtParam.Columns.Add(new System.Data.DataColumn("ControlName", typeof(string)));
				dtParam.Columns.Add(new System.Data.DataColumn("Value", typeof(string)));
			}
			res.Tables.Add(dtParam);

			try
			{
				if (dsParam.Tables.Contains("Params") && (1 == dsParam.Tables["Params"].Rows.Count))
				{
					string sControlName = dsParam.Tables["Params"].Rows[0]["ControlName"].ToString();

                    Point3d? pt = framework.autocad.Utility.PickPoint();
					if (pt.HasValue)
					{
						DataRow drParam = res.Tables["Result"].NewRow();
						{
							drParam["ControlName"] = sControlName;
                            drParam["Value"] = string.Format("{0:F3},{1:F3}", pt.Value.X, pt.Value.Y);
						}
						res.Tables["Result"].Rows.Add(drParam);
					}
				}
			}
			catch { }

			return res;
		}
    }
}
