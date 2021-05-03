using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	public class pJ_genGroup
	{
		public static List<pJ_genGroup> pJ_genGroupList = new List<pJ_genGroup>();

		public string Name{get;set;}
		public string GroupType { get; set; }
		public ObjectIdCollection EntityObjIdList = new ObjectIdCollection();
		public List<TypedValue> TypeValueList = new List<TypedValue>();

		/// <summary>
		/// create pJ_genGroup instance from given AutoCAD group
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.12.15</date>
		/// <param name="aGroup"></param>
		/// <returns></returns>
		public static int CreateInst(Transaction tr , Group aGroup)
		{
			string sGroupName = string.Empty, sGroupType = string.Empty;

			using (var rb = aGroup.XData)
			{
				if (rb == null) return 0;
				{
					TypedValue tmp = rb.AsArray()[0];
					if ((int)DxfCode.ExtendedDataRegAppName == tmp.TypeCode)
					{
						sGroupType = tmp.Value.ToString();
					}

					if ("WM_Joint" == sGroupType)
					{
						Dictionary<string, string> xdata = framework.autocad.AcExDbDBObject.GetXDataAsDictionary(aGroup, sGroupType);
						string[] oXdata = new string[3] { "BOM_DATA", string.Empty, string.Empty };

						sGroupName = "WELD";
						if (xdata.ContainsKey("JointNo"))
						{
							sGroupName += xdata["JointNo"];
						}
						if (xdata.ContainsKey("JointRevType"))
						{
							sGroupName += xdata["JointRevType"];
						}

						if (xdata.ContainsKey("BOM1_PartNo"))
						{
							oXdata[1] += xdata["BOM1_PartNo"];
						}

						if (xdata.ContainsKey("Spool1_IsoDwgNo"))
						{
							oXdata[2] += xdata["Spool1_IsoDwgNo"];
						}
						if (xdata.ContainsKey("Spool1_SpoolNo"))
						{
							if (!string.IsNullOrEmpty(oXdata[2]) && !string.IsNullOrEmpty(xdata["Spool1_SpoolNo"])) oXdata[2] += "-";
							oXdata[2] += xdata["Spool1_SpoolNo"];
						}
						if (xdata.ContainsKey("ConnType"))
						{
							if ("Support" == xdata["ConnType"]) sGroupType = "Support";
						}

						pJ_genGroup apJ_genGroup = new pJ_genGroup() { GroupType = sGroupType };
						{
							apJ_genGroup.Name = sGroupName;
							foreach (ObjectId objId in aGroup.GetAllEntityIds())
							{
								using (Entity acEnt = tr.GetObject(objId, OpenMode.ForWrite) as Entity)
								{
									if(null != acEnt) acEnt.Layer = "Welding";
								}

								apJ_genGroup.EntityObjIdList.Add(objId);
							}
							apJ_genGroup.TypeValueList.Add(new TypedValue((int)DxfCode.ExtendedDataRegAppName, oXdata[0]));
							apJ_genGroup.TypeValueList.Add(new TypedValue((int)DxfCode.ExtendedDataAsciiString, oXdata[1]));
							apJ_genGroup.TypeValueList.Add(new TypedValue((int)DxfCode.ExtendedDataAsciiString, oXdata[2]));
						}
						pJ_genGroupList.Add(apJ_genGroup);
					}
					else if ("WM_Spool" == sGroupType)
					{
						Dictionary<string, string> xdata = framework.autocad.AcExDbDBObject.GetXDataAsDictionary(aGroup, sGroupType);
						sGroupName = "SPOO";

						if (xdata.ContainsKey("SpoolNo"))
						{
							sGroupName += xdata["SpoolNo"];
						}

						pJ_genGroup apJ_genGroup = new pJ_genGroup() { GroupType = sGroupType };
						{
							apJ_genGroup.Name = sGroupName;
							foreach (ObjectId objId in aGroup.GetAllEntityIds())
							{
								using (Entity acEnt = tr.GetObject(objId, OpenMode.ForWrite) as Entity)
								{
									if (null != acEnt) acEnt.Layer = "Welding";
								}
								apJ_genGroup.EntityObjIdList.Add(objId);
							}
						}
						pJ_genGroupList.Add(apJ_genGroup);
					}
					else if ("WM_MarkupRevStamp" == aGroup.Name)
					{
						sGroupName = "INSERTDATA";
						pJ_genGroup apJ_genGroup = new pJ_genGroup() { GroupType = string.Empty };
						{
							apJ_genGroup.Name = sGroupName;
							foreach (ObjectId objId in aGroup.GetAllEntityIds())
							{
								using(Entity acEnt = tr.GetObject(objId , OpenMode.ForWrite) as Entity)
								{
									if (null != acEnt) acEnt.Layer = "INSERTDATA";
								}

								apJ_genGroup.EntityObjIdList.Add(objId);
							}
						}
						pJ_genGroupList.Add(apJ_genGroup);
					}
				}
			}

			return 0;
		}

		/// <summary>
		/// create autocad group for pJ-gen
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.12.15</date>
		/// <returns></returns>
		public static int CreateGroup(Database database , Editor ed)
		{
			using(Transaction tr = database.TransactionManager.StartTransaction())
			{
				BlockTable blockTable = (BlockTable)tr.GetObject(database.BlockTableId, OpenMode.ForRead);
				BlockTableRecord acBlkTblRcd = (BlockTableRecord)tr.GetObject(((SymbolTable)blockTable)[BlockTableRecord.ModelSpace], OpenMode.ForWrite);

				/// Get the group dictionary from the drawing
				DBDictionary gd = (DBDictionary)tr.GetObject(database.GroupDictionaryId, OpenMode.ForRead);

				/// create groups for pJ-gen
				foreach (pJ_genGroup inst in pJ_genGroupList)
				{
					try
					{
						if (0 == inst.EntityObjIdList.Count) continue;

						/// Only set the block name if it isn't in use
						if (gd.Contains(inst.Name))
						{
							ed.WriteMessage(string.Format("\n{0} : A group with this name({1}) already exists.", System.IO.Path.GetFileName(database.Filename), inst.Name));
							continue;
						}

						/// Create our new group...
						Group acGrp = new Group("pJ-gen group", true);

						/// Add the new group to the dictionary
						gd.UpgradeOpen();
						ObjectId grpId = gd.SetAt(inst.Name, acGrp);
						tr.AddNewlyCreatedDBObject(acGrp, true);

						/// insert graphic object to group
						ObjectIdCollection oObjIdColl = new ObjectIdCollection();
						Extents3d acExt = new Extents3d();
						string sLayer = string.Empty;
						int iColorIndex = 0;
						foreach (ObjectId acObjId in inst.EntityObjIdList)
						{
							using (Entity acEnt = tr.GetObject(acObjId, OpenMode.ForWrite) as Entity)
							{
								/// change entities for pj-gen(polyline --> circle,line,line --> leader)
								if (acEnt is Polyline)
								{
									Polyline acPoly = acEnt as Polyline;
									if (2 == acPoly.NumberOfVertices)
									{
										Line acLine = new Line(acPoly.GetPoint3dAt(0), acPoly.GetPoint3dAt(1))
										{
											Layer = acEnt.Layer,
											ColorIndex = acEnt.ColorIndex
										};
										oObjIdColl.Add(acBlkTblRcd.AppendEntity(acLine));
										tr.AddNewlyCreatedDBObject(acLine, true);

										acEnt.Erase(true);
									}
									else if (acPoly.NumberOfVertices > 2)
									{
										if ("WM_Joint" == inst.GroupType)
										{
											Point3d center = new Point3d((acPoly.GetPoint3dAt(0).X + acPoly.GetPoint3dAt(2).X) * 0.5, (acPoly.GetPoint3dAt(0).Y + acPoly.GetPoint3dAt(2).Y) * 0.5, (acPoly.GetPoint3dAt(0).Z + acPoly.GetPoint3dAt(2).Z) * 0.5);
											double radius = acPoly.GetPoint3dAt(0).DistanceTo(acPoly.GetPoint3dAt(2)) * 0.5;
											Circle acCircle = new Circle(center, Vector3d.ZAxis, radius)
											{
												Layer = acEnt.Layer,
												ColorIndex = acEnt.ColorIndex
											};
											oObjIdColl.Add(acBlkTblRcd.AppendEntity(acCircle));
											tr.AddNewlyCreatedDBObject(acCircle, true);
										}
										else if ("WM_Spool" == inst.GroupType)
										{
											acExt.AddExtents(acPoly.GeometricExtents);
											sLayer = acPoly.Layer;
											iColorIndex = acPoly.ColorIndex;
										}

										if ("Support" == inst.GroupType)
										{
											oObjIdColl.Add(acObjId);
										}
										else
										{
											acEnt.Erase(true);
										}
									}
								}
								else if (acEnt is Line)
								{
									Line acLine = acEnt as Line;
									Leader acLeader = new Leader()
									{
										Layer = acLine.Layer,
										ColorIndex = acLine.ColorIndex,
										Dimclrd = acLine.Color
									};
									acLeader.AppendVertex(acLine.StartPoint);
									if (acLine.Length > 0)
									{
										acLeader.AppendVertex(acLine.EndPoint);
									}
									else
									{
										acLeader.AppendVertex(new Point3d(acLine.StartPoint.X + 0.005, acLine.StartPoint.Y + 0.005, acLine.StartPoint.Z));
									}
									acLeader.HasArrowHead = false;

									oObjIdColl.Add(acBlkTblRcd.AppendEntity(acLeader));
									tr.AddNewlyCreatedDBObject(acLeader, true);

									acEnt.Erase(true);
								}
								else
								{
									oObjIdColl.Add(acObjId);
								}
							}
						}
						if ("WM_Spool" == inst.GroupType)
						{
							Polyline acPoly = new Polyline(4)
							{
								Layer = sLayer,
								ColorIndex = iColorIndex,
								Closed = true,
							};
							double dBorderWidth = (acExt.MaxPoint.Y - acExt.MinPoint.Y)/10;

							acPoly.AddVertexAt(0, new Point2d(acExt.MinPoint.X, acExt.MinPoint.Y), 0, dBorderWidth, dBorderWidth);
							acPoly.AddVertexAt(1, new Point2d(acExt.MaxPoint.X, acExt.MinPoint.Y), 0, dBorderWidth, dBorderWidth);
							acPoly.AddVertexAt(2, new Point2d(acExt.MaxPoint.X, acExt.MaxPoint.Y), 0, dBorderWidth, dBorderWidth);
							acPoly.AddVertexAt(3, new Point2d(acExt.MinPoint.X, acExt.MaxPoint.Y), 0, dBorderWidth, dBorderWidth);
							oObjIdColl.Add(acBlkTblRcd.AppendEntity(acPoly));
							tr.AddNewlyCreatedDBObject(acPoly, true);
						}

						acGrp.InsertAt(0, oObjIdColl);

						/// add xdata
						ResultBuffer rb = new ResultBuffer();
						foreach (TypedValue aTypeValue in inst.TypeValueList)
						{
							rb.Add(aTypeValue);
						}
						acGrp.XData = rb;
						rb.Dispose();
					}
					catch (System.Exception ex)
					{
						ed.WriteMessage(Environment.NewLine + ex.StackTrace);
					}
				}

				/// Commit the transaction
				tr.Commit();
			}

			return 0;
		}
	}
}
