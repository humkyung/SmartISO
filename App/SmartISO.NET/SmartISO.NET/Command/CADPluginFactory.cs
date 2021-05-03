using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Data;
using System.IO;

namespace SmartISO.NET
{
	public class CADPluginFactory
	{
		public static ICADPluginCommand Create(DataSet dsParam)
		{
			if (dsParam.Tables.Contains("Command") && (1 == dsParam.Tables["Command"].Rows.Count))
			{	
				string sCommandName = dsParam.Tables["Command"].Rows[0]["Name"].ToString();
				if("Echo" == sCommandName)
				{
					return new CADPluginEchoCommand();
				}
				else if("OpenDwg" == sCommandName)
				{
					return new CADPluginOpenDwgCommand();
				}
                else if ("PickPoint" == sCommandName)
                {
                    return new CADPluginPickPointCommand();
                }
				else if ("SelectBounds" == sCommandName)
				{
					return new CADPluginSelectBoundsCommand();
				}
                else if ("GetDistance" == sCommandName)
                {
                    return new CADPluginGetDistanceCommand();
                }
				else
				{
					return new CADPluginUnExpectedCommand();
				}
			}

			return null;
		}
	}
}
