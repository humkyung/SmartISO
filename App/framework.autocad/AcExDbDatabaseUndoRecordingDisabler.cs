using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;

namespace framework.autocad
{
	class AcExDbDatabaseUndoRecordingDisabler : IDisposable
	{
		private Database db_;
		private bool UndoRecording_;

		public AcExDbDatabaseUndoRecordingDisabler(Database db)
		{
			this.db_ = db;
			this.UndoRecording_ = db.UndoRecording;
			this.db_.DisableUndoRecording(true);
		}

		public void Dispose()
		{
			this.db_.DisableUndoRecording(!this.UndoRecording_);
		}	
	}
}
