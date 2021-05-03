using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Colors;

namespace framework.autocad
{
	public class AcExDbTextStyle
	{
		private AcExDbTextStyle()
		{
		}

		public static void Create(string strTxtStyleName, string strFont, double dH, bool bBold, bool bItalic, Database db)
		{
			if (null == db) db = HostApplicationServices.WorkingDatabase;
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				TextStyleTable acTxtStyleTbl = (TextStyleTable)transaction.GetObject(db.TextStyleTableId, OpenMode.ForWrite);
				TextStyleTableRecord acTxtStyleTblRcd = new TextStyleTableRecord() { Name = strTxtStyleName };
				acTxtStyleTblRcd.Font = new Autodesk.AutoCAD.GraphicsInterface.FontDescriptor(strFont, bBold, bItalic, 0, 0);
				acTxtStyleTblRcd.TextSize = dH;
				acTxtStyleTbl.Add(acTxtStyleTblRcd);
				transaction.AddNewlyCreatedDBObject(acTxtStyleTblRcd, true);

				transaction.Commit();
			}
		}
	}
}
