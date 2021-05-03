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

[assembly: CommandClass(typeof(SmartISO.NET.CADPluginSelectBoundsCommand))]
namespace SmartISO.NET
{
	public class CADPluginSelectBoundsCommand : ICADPluginCommand
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

					AcExDbBounds oBounds = framework.autocad.Utility.SelectBounds();
					if (null != oBounds)
					{
						DataRow drParam = res.Tables["Result"].NewRow();
						{
							drParam["ControlName"] = sControlName;
							drParam["Value"] = oBounds.ToString();
						}
						res.Tables["Result"].Rows.Add(drParam);

						Return(res);
					}
				}
			}
			catch { }

			return res;
		}

		private static void Return(DataSet dsParam)
		{
			try
			{
				using (TcpClient tcpClient = new TcpClient("127.0.0.1", 9328))
				{
					using (NetworkStream stream = tcpClient.GetStream())
					{
						if (null != dsParam)
						{
							StringWriter sw = new StringWriter();
							dsParam.WriteXml(sw);
							byte[] oDataSize = BitConverter.GetBytes(sw.ToString().Length);
							byte[] buffer = Encoding.UTF8.GetBytes(sw.ToString());
							byte[] oResult = new byte[oDataSize.Length + buffer.Length];
							System.Buffer.BlockCopy(oDataSize, 0, oResult, 0, oDataSize.Length);
							System.Buffer.BlockCopy(buffer, 0, oResult, oDataSize.Length, buffer.Length);
							stream.Write(oResult, 0, oResult.Length);
							stream.Flush();
						}
					}
				}
			}
			catch (System.Exception ex)
			{
				throw ex;
			}
		}
	}
}
