using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using IDCS.DataTransferObjects;
using EDCS.WF.DSL;
using System.Data;
using CommonLibrary.CL;

namespace Configuration
{
    public class SvcProxy
    {
        public static List<EntitySPTool_Project> GetSP3DProjectInfo()
        {
            List<EntitySPTool_Project> oProjectList = new List<EntitySPTool_Project>();
            string sEndPointAddress = AppDocData.ServiceURL;
            using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "GetSP3DProjectInfoCommand"))
                {
                    using (DataSet ds = svcProxy.ExecuteCommand("SP3D.BSL", "GetSP3DProjectInfoCommand", dsParam))
                    {
                        foreach (DataRow oRow in ds.Tables[0].Rows)
                        {
                            oProjectList.Add(new EntitySPTool_Project()
                            {
                                ProjectID = System.Convert.ToInt32(oRow["ProjectID"]),
                                ProjectMasterID = System.Convert.ToInt32(oRow["ProjectMasterID"]),
                                ProjectNo = oRow["ProjectNo"].ToString(),
                                ProjectName = oRow["ProjectName"].ToString(),
                                DBHost = oRow["DBHost"].ToString(),
                                DBPort = oRow["DBPort"].ToString(),
                                DBService = oRow["DBService"].ToString(),
                                DBUserID = oRow["DBUserID"].ToString(),
                                DBUserPW = oRow["DBUserPW"].ToString(),
                                DBSchema = oRow["DBSchema"].ToString(),
                                DeleteFlag = (true.ToString() == oRow["DeleteFlag"].ToString())
                            });
                        }
                    }
                }
            }

            return oProjectList;
        }

        /// <summary>
        /// 일배치로 SP3D PipeRun 데이터를 EDB 서벌SP3D 테이블로 저장한다.
        /// </summary>
        /// <author>kwon hyunseok</author>
        /// <date>2016,09,07</date>
        public static void InsertSP3DData(DataTable p_dataTable, string p_tableName, string p_projectMasterID)
        {
            string sEndPointAddress = AppDocData.ServiceURL;
            using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "InsertSP3DDataCommand"))
                {
                    DataRow drParam = dsParam.Tables["Params"].NewRow();
                    drParam["ProjectMasterID"] = System.Convert.ToInt32(p_projectMasterID);
                    drParam["TableName"] = p_tableName;
                    dsParam.Tables["Params"].Rows.Add(drParam);

                    p_dataTable.TableName = p_tableName;
                    dsParam.Tables.Add(p_dataTable);

                    using (DataSet ds = svcProxy.ExecuteCommand("SP3D.BSL", "InsertSP3DDataCommand", dsParam))
                    {

                    }
                }
            }
        }

        public static DataSet GetSp3dConfig(string p_projectID, string p_category, string p_key)
        {
            DataSet ds = null;
			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "GetSp3dConfigCommand"))
                {
                    DataRow drParam = dsParam.Tables["Params"].NewRow();
                    drParam["PrjCode"] = p_projectID;
                    drParam["Section"] = p_category;
                    drParam["Key"] = p_key;
                    dsParam.Tables["Params"].Rows.Add(drParam);

                    ds = svcProxy.ExecuteCommand("SP3D.BSL", "GetSp3dConfigCommand", dsParam);

                }
            }

            return ds;
        }

        public static void SaveSp3dConfigs(List<ProjectConfigDTO> oProjectConfigDTO, string p_projectID)
        {
			using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "SaveSp3dConfigsCommand"))
                {
                    foreach (ProjectConfigDTO config in oProjectConfigDTO)
                    {
                        DataRow drParam = dsParam.Tables["Params"].NewRow();
						drParam["Section"] = config.Section;
                        drParam["Key"] = config.Key;
                        drParam["Value"] = config.Value;
						drParam["ProjectMasterID"] = p_projectID;
                        dsParam.Tables["Params"].Rows.Add(drParam);
                    }

                    svcProxy.ExecuteCommand("SP3D.BSL", "SaveSp3dConfigsCommand", dsParam);

                }
            }
        }

        public static List<EntityCommon_PBSMapping> GetPBS()
        {
            List<EntityCommon_PBSMapping> oPBSList = new List<EntityCommon_PBSMapping>();

            string sEndPointAddress = AppDocData.ServiceURL;
            using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "GetPBSCommand"))
                {
                    using (DataSet ds = svcProxy.ExecuteCommand("SP3D.BSL", "GetPBSCommand", dsParam))
                    {
                        foreach (DataRow oRow in ds.Tables[0].Rows)
                        {
                            oPBSList.Add(new EntityCommon_PBSMapping()
                            {
                                PBSMappingID = System.Convert.ToInt32(oRow["PBSMappingID"]),
                                ProjectMasterID = System.Convert.ToInt32(oRow["ProjectMasterID"]),
                                Type = oRow["Type"].ToString(),
                                SPPID_Name = oRow["SPPID_Name"].ToString(),
                                SP3D_Name = oRow["SP3D_Name"].ToString()
                            });
                        }
                    }
                }
            }

            return oPBSList;
        }


        public static DataSet GetPipeLineDataSP3D(string p_projectMasterID, string p_unit)
        {
            DataSet ds = null;

            string sEndPointAddress = AppDocData.ServiceURL;
            using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "GetPipeLineDataSP3DCommand"))
                {
                    DataRow drParam = dsParam.Tables["Params"].NewRow();
                    drParam["ProjectMasterID"] = System.Convert.ToInt32(p_projectMasterID);
                    drParam["Unit"] = p_unit;
                    dsParam.Tables["Params"].Rows.Add(drParam);

                    ds = svcProxy.ExecuteCommand("SP3D.BSL", "GetPipeLineDataSP3DCommand", dsParam);
                }
            }

            return ds;
        }

        public static DataSet DownloadReport(string p_fileName)
        {
            DataSet ds = null;

            string sEndPointAddress = AppDocData.ServiceURL;
            using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SP3D.BSL", "DownloadReportCommand"))
                {
                    //dsParam.Tables.Add("Params");
                    DataRow drParam = dsParam.Tables["Params"].NewRow();
                    drParam["Name"] = p_fileName;

                    ds = svcProxy.ExecuteCommand("SP3D.BSL", "DownloadReportCommand", dsParam);
                }
            }

            return ds;
        }

        public static List<ProjectDTO> GetAllProjectList()
        {
            List<ProjectDTO> oProjectList = new List<ProjectDTO>();
            string sEndPointAddress = AppDocData.ServiceURL;
            using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("IDCS.BSL", "GetAllProjectListCommand"))
                {
                    using (DataSet ds = svcProxy.ExecuteCommand("IDCS.BSL", "GetAllProjectListCommand", dsParam))
                    {
                        foreach (DataRow oRow in ds.Tables[0].Rows)
                        {
                            oProjectList.Add(
                                new ProjectDTO()
                                {
                                    Id = Convert.ToInt32(oRow["ProjectMasterID"]),
                                    Name = oRow["ProjectNo"].ToString(),
                                    Desc = oRow["NameView"].ToString(),
                                    Enabled = (true.ToString() == oRow["SP3DFlag"].ToString())
                                }
                            );
                        }
                    }
                }
            }

            return oProjectList;
        }
    }
}
