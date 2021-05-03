using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Net.Sockets;
using System.IO;

namespace SmartISO.NET
{
	public interface ICADPluginCommand
	{
		DataSet Execute(DataSet dsParam);
	}
}
