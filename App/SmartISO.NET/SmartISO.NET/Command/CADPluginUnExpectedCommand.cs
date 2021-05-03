using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

namespace SmartISO.NET
{
	public class CADPluginUnExpectedCommand : ICADPluginCommand
	{
		public DataSet Execute(DataSet dsParam)
		{
			DataSet res = new DataSet();
			DataTable dtParam = new DataTable("Result");
			{
				dtParam.Columns.Add(new DataColumn("Value", typeof(string)));
			}
			res.Tables.Add(dtParam);

			DataRow drParam = res.Tables["Result"].NewRow();
			{
				drParam["Value"] = "UnExpected Command";
			}
			res.Tables["Result"].Rows.Add(drParam);

			return res;
		}
	}
}
