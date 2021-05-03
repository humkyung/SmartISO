using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IsoDwg.NET
{
	interface AbstractCommand
	{
		List<QCCheckCommand.Result> Execute(Autodesk.AutoCAD.ApplicationServices.Document doc);
		List<QCCheckCommand.Result> Execute(IsoDwgParser parser);
	}
}
