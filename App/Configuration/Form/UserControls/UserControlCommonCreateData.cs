using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using EDCS.WF.DSL;
using System.IO;
using IDCS.Framework.Service;
using IDCS.Service.Interface;
using IDCS.ServiceImpl;
using Configuration.Properties;
using EDCS.WF.BSL;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlCommonCreateData : UserControlBase
    {
        public UserControlCommonCreateData()
        {
            InitializeComponent();

            this.radButtonSP3DDownload.Click += new System.EventHandler(this.radButtonSP3DDownload_Click);
            this.radButtonSPPIDDownload.Click += new System.EventHandler(this.radButtonSPPIDDownload_Click);
        }

        private void InitDropDownControl()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectSP3D, this._entityEDBInformation, ToolType.SP3D);
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectSPPID, this._entityEDBInformation, ToolType.SPPID);
        }

        public override void LoadData()
        {
            InitDropDownControl();
        }

        private void radButtonSP3DDownload_Click(object sender, EventArgs e)
        {
            var item = (EntitySPTool_Project)radDropDownListProjectSPPID.SelectedItem.DataBoundItem;

            if (-1 != item.ProjectMasterID)
            {
                //using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(Common.endPointAddress))
                {
                    // 3D DB 정보를 가져옴
                    var sp3dProject = SvcProxy.GetSP3DProjectInfo().Where(w => w.ProjectMasterID == item.ProjectMasterID).FirstOrDefault();
                    if (null != sp3dProject)
                    {
                        DataTable pipeLineTable = SP3DModel.GetPipeLineAttributeDataSP3D(sp3dProject.DBHost, sp3dProject.DBPort, sp3dProject.DBService, sp3dProject.DBUserID, sp3dProject.DBUserPW, sp3dProject.DBSchema, item.ProjectMasterID.ToString());
                        DataTable equipmentTable = SP3DModel.GetEquipmentAttributeDataSP3D(sp3dProject.DBHost, sp3dProject.DBPort, sp3dProject.DBService, sp3dProject.DBUserID, sp3dProject.DBUserPW, sp3dProject.DBSchema, item.ProjectMasterID.ToString());
                        DataTable instrumentTable = SP3DModel.GetInstrumentAttributeDataSP3D(sp3dProject.DBHost, sp3dProject.DBPort, sp3dProject.DBService, sp3dProject.DBUserID, sp3dProject.DBUserPW, sp3dProject.DBSchema, item.ProjectMasterID.ToString());

                        //string sEndPointAddress = AppDocData.ServiceURL + "SP3DService.svc";
                        //using (ISP3DService svcSP3DProxy = ServiceFactory.CreateServiceChannel<ISP3DService>(sEndPointAddress))
                        {
                            SvcProxy.InsertSP3DData(equipmentTable, "SP3D_EQUIPMENT", item.ProjectMasterID.ToString());
                            SvcProxy.InsertSP3DData(instrumentTable, "SP3D_INSTRUMENT", item.ProjectMasterID.ToString());
                            SvcProxy.InsertSP3DData(pipeLineTable, "SP3D_PIPERUN", item.ProjectMasterID.ToString());
                        }
                    }
                }
            }
        }

        private void radButtonSPPIDDownload_Click(object sender, EventArgs e)
        {

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Create DataValue.  Please wait!";

            List<string> lstError = new List<string>();
            try
            {
                string sError = string.Empty;

                List<string> lstItemType = Information.ITEMTYPE;

                Information information = new Information();

                information.Project= (EntitySPTool_Project)radDropDownListProjectSPPID.SelectedItem.DataBoundItem;

                information.Plant = null;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<InfoPlant> lstPlant = proxy.GetPlantNames(EDCS.WF.BSL.Variable.SharedVariable.IsIE, information, information.Project, GeneralUtil.InitConnection(information));

                    foreach (var itemPlant in lstPlant)
                    {
                        information.Plant = itemPlant;
                        information.InfoDB = new InfoDB();
                        information.InfoDB.DBPath = Path.Combine(Paths.PATH_DATA, information.Project.ProjectNo, information.Plant.SiteName);

                        DataSet dsConfig = proxy.GetConfigData(EDCS.WF.BSL.Variable.SharedVariable.IsIE, information, lstItemType);

                        foreach (string sItemType in lstItemType)
                        {
                            FrmProgressStatus.ProgressPer = string.Format("[{0}] Create DataValue.[{1}]  Please wait!", itemPlant.SiteName, sItemType);

                            proxy.CreateDataValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, information, sItemType, dsConfig);

                            if (sError.Length > 0)
                            {
                                lstError.Add(sError);
                            }
                        }

                        foreach (DataTable dt in dsConfig.Tables)
                        {
                            dt.Dispose();
                        }

                        dsConfig.Dispose();
                    }
                }
            }
            catch (System.ServiceModel.FaultException ex)
            {
                FrmProgressStatus.FormClose();
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }

            string sMessage = "success";

            foreach (string sErr in lstError)
            {
                sMessage += "\r\n " + sErr;
            }

            MessageBox.Show(sMessage);

            FrmProgressStatus.FormClose();
        }

        public class SP3DModel
        {
            private static DataTable CreateCommand(string p_query, AppOracleDatabase p_oracle, string p_rDBSchema)
            {
                string rQuery = string.Format("ALTER SESSION SET CURRENT_SCHEMA = {0}", p_rDBSchema);
                p_oracle.ExecuteNonQuery(rQuery);

                using (IDataReader reader = p_oracle.ExecuteReader(p_oracle.GetSqlStringCommand(p_query)))
                {
                    DataTable dataTable = new DataTable("Table1");
                    dataTable.Load(reader);

                    reader.Close();
                    return dataTable;
                }
            }
            // Plant
            public static DataTable GetPlant(string p_host, string p_service, string p_id, string p_password, string p_dbSchema)
            {
                DataTable dataTable = null;
                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rQuery = @"Select cast(x1.oid as varchar(256)) as OID, n1.ItemName as ITEMNAME from JConfigProjectRoot x1
                             join JNamedItem n1 on n1.Oid = x1.oid";

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            // PipeRun
            public static DataTable GetAllPipeRunSP3D(string p_host, string p_service, string p_id, string p_password, string p_dbSchema, string p_unit)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rQuery = string.Format(@"select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
join JPipingSystem PS on PS.Oid = x7.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join JPipingSystem PS on PS.Oid = x3.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join JPipingSystem PS on PS.Oid = x5.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join JPipingSystem PS on PS.Oid = x6.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}' order by PIPELINE, NPD DESC, PIPERUN", p_unit); // FCC용 쿼리

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                        dataTable.Columns.Add("DRAWINGPATH");
                        dataTable.Columns.Add("PIPERUNSPID");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            // PipeLine
            public static DataTable GetPipeLineDataSP3D(string p_host, string p_service, string p_id, string p_password, string p_dbSchema, string p_unit)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rQuery = string.Format(@"select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast(PT.ShortCode as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
join JPipingSystem PS on PS.Oid = x7.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRtePartData PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Specialty Item' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
join JPipingSystem PS on PS.Oid = x7.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteSpecialtyOccur PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Instrument' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
join JPipingSystem PS on PS.Oid = x7.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteInstrument PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Instrument' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join JPipingSystem PS on PS.Oid = x3.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteInstrument PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast(PT.ShortCode as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join JPipingSystem PS on PS.Oid = x3.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRtePartData PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Specialty Item' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join JPipingSystem PS on PS.Oid = x3.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteSpecialtyOccur PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast(PT.ShortCode as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join JPipingSystem PS on PS.Oid = x5.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRtePartData PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Specialty Item' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join JPipingSystem PS on PS.Oid = x5.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteSpecialtyOccur PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Instrument' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join JPipingSystem PS on PS.Oid = x5.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteInstrument PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast(PT.ShortCode as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join JPipingSystem PS on PS.Oid = x6.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRtePartData PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Specialty Item' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join JPipingSystem PS on PS.Oid = x6.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteSpecialtyOccur PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast(N3.ItemName as VARCHAR(256)) as PART,
cast((SELECT 'Instrument' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join JPipingSystem PS on PS.Oid = x6.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join XOwnsParts x2 on x2.OidOrigin = PR.Oid
join JRteInstrument PT on PT.Oid = x2.OidDestination
join JNamedItem N3 on N3.Oid = PT.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
join JPipingSystem PS on PS.Oid = x7.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join JPipingSystem PS on PS.Oid = x3.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join JPipingSystem PS on PS.Oid = x5.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'
UNION
select
N4.itemname as UNIT,
cast(PL.oid as varchar(256)) as OID,
N.itemname as PIPELINE,
cast(PR.oid as varchar(256)) as OID,
N2.ItemName as PIPERUN,
cast(PR.NPD as REAL) as NPD,
PR.NPDUnitType as MEASURE,
cast((SELECT '' from DUAL) as VARCHAR(256)) as PART,
cast((SELECT '' from DUAL) as VARCHAR(256)) as MAPPINGTYPE
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join JPipingSystem PS on PS.Oid = x6.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
where N4.ItemName = '{0}'", p_unit); // FCC용 쿼리

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                        dataTable.Columns.Add("DRAWINGPATH");
                        dataTable.Columns.Add("PIPERUNSPID");
                        dataTable.Columns.Add("LABEL");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            // PipeRun
            public static DataTable GetPipeRunSP3D(string p_host, string p_service, string p_id, string p_password, string p_dbSchema)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {

                        string rQuery = @"select
distinct
N.itemname as PIPERUN
from JRtePipeRun PR
join JNamedItem N on N.Oid = PR.Oid
order by PIPERUN";

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            // PipeLine Attribute
            public static DataTable GetPipeLineAttributeDataSP3D(string p_host, string p_service, string p_id, string p_password, string p_dbSchema, List<string> p_units)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rUnits = string.Empty;
                        foreach (var unit in p_units)
                            rUnits += ("'" + unit.Trim() + "',");
                        rUnits = rUnits.Substring(0, rUnits.Length - 1);

                        string rQuery = string.Format(@"select
distinct
N4.itemname as UNIT,
cast(PR.oid as varchar(256)) as OID,
N2.itemname as PIPERUN,
x2.DESIGN_PRESS,
x2.OPER_PRESS,
x2.TEST_PRESS,
x2.DESIGN_TEMP,
x2.OPER_TEMP,
x2.INSUL_CODE,
x2.INSUL_THK,
x2.PAINT_CODE,
x2.RTPT,
x2.PWHT,
x2.TEST_MEDIUM,
x2.STEAM_TRACE,
x2.STEAM_JACKETING,
x2.ELECTRIC_TRACE,
x2.STRESS_ANALYSIS,
x2.CLEANING,
x2.PIPING_PID,
x2.PIPING_PLAN,
x2.PIPING_STRESS,
cast(PR.NPD as REAL) as NPD,
PL.FLUIDCODE as FLUIDCODE,
PL.SEQUENCENUMBER,
x8.HeatTracingMedium
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
join JPipingSystem PS on PS.Oid = x7.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join JUADICPipeRun x2 on x2.oid = PR.oid
join JHeatTracingInfo x8 on x8.oid = PR.oid
where N4.ItemName in ({0})
UNION
select
distinct
N4.itemname as UNIT,
cast(PR.oid as varchar(256)) as OID,
N2.itemname as PIPERUN,
x2.DESIGN_PRESS,
x2.OPER_PRESS,
x2.TEST_PRESS,
x2.DESIGN_TEMP,
x2.OPER_TEMP,
x2.INSUL_CODE,
x2.INSUL_THK,
x2.PAINT_CODE,
x2.RTPT,
x2.PWHT,
x2.TEST_MEDIUM,
x2.STEAM_TRACE,
x2.STEAM_JACKETING,
x2.ELECTRIC_TRACE,
x2.STRESS_ANALYSIS,
x2.CLEANING,
x2.PIPING_PID,
x2.PIPING_PLAN,
x2.PIPING_STRESS,
cast(PR.NPD as REAL) as NPD,
PL.FLUIDCODE as FLUIDCODE,
PL.SEQUENCENUMBER,
x8.HeatTracingMedium
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
join JPipingSystem PS on PS.Oid = x3.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join JUADICPipeRun x2 on x2.oid = PR.oid
join JHeatTracingInfo x8 on x8.oid = PR.oid
where N4.ItemName in ({0})
UNION
select
distinct
N4.itemname as UNIT,
cast(PR.oid as varchar(256)) as OID,
N2.itemname as PIPERUN,
x2.DESIGN_PRESS,
x2.OPER_PRESS,
x2.TEST_PRESS,
x2.DESIGN_TEMP,
x2.OPER_TEMP,
x2.INSUL_CODE,
x2.INSUL_THK,
x2.PAINT_CODE,
x2.RTPT,
x2.PWHT,
x2.TEST_MEDIUM,
x2.STEAM_TRACE,
x2.STEAM_JACKETING,
x2.ELECTRIC_TRACE,
x2.STRESS_ANALYSIS,
x2.CLEANING,
x2.PIPING_PID,
x2.PIPING_PLAN,
x2.PIPING_STRESS,
cast(PR.NPD as REAL) as NPD,
PL.FLUIDCODE as FLUIDCODE,
PL.SEQUENCENUMBER,
x8.HeatTracingMedium
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
join JPipingSystem PS on PS.Oid = x5.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join JUADICPipeRun x2 on x2.oid = PR.oid
join JHeatTracingInfo x8 on x8.oid = PR.oid
where N4.ItemName in ({0})
UNION
select
distinct
N4.itemname as UNIT,
cast(PR.oid as varchar(256)) as OID,
N2.itemname as PIPERUN,
x2.DESIGN_PRESS,
x2.OPER_PRESS,
x2.TEST_PRESS,
x2.DESIGN_TEMP,
x2.OPER_TEMP,
x2.INSUL_CODE,
x2.INSUL_THK,
x2.PAINT_CODE,
x2.RTPT,
x2.PWHT,
x2.TEST_MEDIUM,
x2.STEAM_TRACE,
x2.STEAM_JACKETING,
x2.ELECTRIC_TRACE,
x2.STRESS_ANALYSIS,
x2.CLEANING,
x2.PIPING_PID,
x2.PIPING_PLAN,
x2.PIPING_STRESS,
cast(PR.NPD as REAL) as NPD,
PL.FLUIDCODE as FLUIDCODE,
PL.SEQUENCENUMBER,
x8.HeatTracingMedium
from JUnitSystem UN
join JNamedItem N4 on N4.Oid = UN.Oid
join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
join JPipingSystem PS on PS.Oid = x6.OidDestination
join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
join JPipelineSystem PL on PL.Oid = x4.OidDestination
join JNamedItem N on N.Oid = PL.Oid
join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
join JRtePipeRun PR on PR.Oid = x1.OidDestination
join JNamedItem N2 on N2.Oid = PR.Oid
join JUADICPipeRun x2 on x2.oid = PR.oid
join JHeatTracingInfo x8 on x8.oid = PR.oid
where N4.ItemName in ({0}) order by PIPERUN", rUnits);

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            public static DataTable GetSp3dReportSheets(string p_host, string p_service, string p_id, string p_password, string p_dbSchema)
            {
                DataTable dataTable = null;
                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rQuery = @"
select 
cast(x1.oid as varchar(256)) as OID, 
x2.itemname as REPORT, 
x4.Name as TYPE, 
x4.DESCRIPTION, 
x4.FILENAME, 
x6.FILENAME as TEMPLATE 
from JDReportsSheet x1
join JNamedItem x2 on x2.oid = x1.oid
join XSheetHasReport x3 on x3.OidDestination = x1.oid 
join JReport x4 on x4.oid = x3.OidOrigin
join XReliesOnTemplate x5 on x5.OidOrigin = x4.oid
join JExternalTemplate x6 on x6.oid = x5.OidDestination";

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            // 디비 쿼리 함수
            public static DataTable GetPipeLineAttributeDataSP3D(string p_host, string p_port, string p_service, string p_id, string p_password, string p_dbSchema, string p_projectMasterID)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rQuery = string.Format(@"
                    select
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(PR.oid as varchar(256)) as OID,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    N2.itemname as ITEMTAG,
                    x2.DESIGN_PRESS,
                    x2.OPER_PRESS,
                    x2.TEST_PRESS,
                    x2.DESIGN_TEMP,
                    x2.OPER_TEMP,
                    x2.INSUL_CODE,
                    x2.INSUL_THK,
                    x2.PAINT_CODE,
                    x2.RTPT,
                    x2.PWHT,
                    x2.TEST_MEDIUM,
                    x2.STEAM_TRACE,
                    x2.STEAM_JACKETING,
                    x2.ELECTRIC_TRACE,
                    x2.STRESS_ANALYSIS,
                    x2.CLEANING,
                    x2.PIPING_PID,
                    x2.PIPING_PLAN,
                    x2.PIPING_STRESS,
                    cast(PR.NPD as REAL) as NPD,
                    PL.FLUIDCODE as FLUIDCODE,
                    PL.SEQUENCENUMBER,
                    x8.HeatTracingMedium
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
                    join JPipingSystem PS on PS.Oid = x7.OidDestination
                    join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
                    join JPipelineSystem PL on PL.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = PL.Oid
                    join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
                    join JRtePipeRun PR on PR.Oid = x1.OidDestination
                    join JNamedItem N2 on N2.Oid = PR.Oid
                    join JUADICPipeRun x2 on x2.oid = PR.oid
                    join JHeatTracingInfo x8 on x8.oid = PR.oid
                    UNION
                    select
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(PR.oid as varchar(256)) as OID,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    N2.itemname as ITEMTAG,
                    x2.DESIGN_PRESS,
                    x2.OPER_PRESS,
                    x2.TEST_PRESS,
                    x2.DESIGN_TEMP,
                    x2.OPER_TEMP,
                    x2.INSUL_CODE,
                    x2.INSUL_THK,
                    x2.PAINT_CODE,
                    x2.RTPT,
                    x2.PWHT,
                    x2.TEST_MEDIUM,
                    x2.STEAM_TRACE,
                    x2.STEAM_JACKETING,
                    x2.ELECTRIC_TRACE,
                    x2.STRESS_ANALYSIS,
                    x2.CLEANING,
                    x2.PIPING_PID,
                    x2.PIPING_PLAN,
                    x2.PIPING_STRESS,
                    cast(PR.NPD as REAL) as NPD,
                    PL.FLUIDCODE as FLUIDCODE,
                    PL.SEQUENCENUMBER,
                    x8.HeatTracingMedium
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join JPipingSystem PS on PS.Oid = x3.OidDestination
                    join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
                    join JPipelineSystem PL on PL.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = PL.Oid
                    join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
                    join JRtePipeRun PR on PR.Oid = x1.OidDestination
                    join JNamedItem N2 on N2.Oid = PR.Oid
                    join JUADICPipeRun x2 on x2.oid = PR.oid
                    join JHeatTracingInfo x8 on x8.oid = PR.oid
                    UNION
                    select
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(PR.oid as varchar(256)) as OID,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    N2.itemname as ITEMTAG,
                    x2.DESIGN_PRESS,
                    x2.OPER_PRESS,
                    x2.TEST_PRESS,
                    x2.DESIGN_TEMP,
                    x2.OPER_TEMP,
                    x2.INSUL_CODE,
                    x2.INSUL_THK,
                    x2.PAINT_CODE,
                    x2.RTPT,
                    x2.PWHT,
                    x2.TEST_MEDIUM,
                    x2.STEAM_TRACE,
                    x2.STEAM_JACKETING,
                    x2.ELECTRIC_TRACE,
                    x2.STRESS_ANALYSIS,
                    x2.CLEANING,
                    x2.PIPING_PID,
                    x2.PIPING_PLAN,
                    x2.PIPING_STRESS,
                    cast(PR.NPD as REAL) as NPD,
                    PL.FLUIDCODE as FLUIDCODE,
                    PL.SEQUENCENUMBER,
                    x8.HeatTracingMedium
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join JPipingSystem PS on PS.Oid = x5.OidDestination
                    join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
                    join JPipelineSystem PL on PL.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = PL.Oid
                    join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
                    join JRtePipeRun PR on PR.Oid = x1.OidDestination
                    join JNamedItem N2 on N2.Oid = PR.Oid
                    join JUADICPipeRun x2 on x2.oid = PR.oid
                    join JHeatTracingInfo x8 on x8.oid = PR.oid
                    UNION
                    select
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(PR.oid as varchar(256)) as OID,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    N2.itemname as ITEMTAG,
                    x2.DESIGN_PRESS,
                    x2.OPER_PRESS,
                    x2.TEST_PRESS,
                    x2.DESIGN_TEMP,
                    x2.OPER_TEMP,
                    x2.INSUL_CODE,
                    x2.INSUL_THK,
                    x2.PAINT_CODE,
                    x2.RTPT,
                    x2.PWHT,
                    x2.TEST_MEDIUM,
                    x2.STEAM_TRACE,
                    x2.STEAM_JACKETING,
                    x2.ELECTRIC_TRACE,
                    x2.STRESS_ANALYSIS,
                    x2.CLEANING,
                    x2.PIPING_PID,
                    x2.PIPING_PLAN,
                    x2.PIPING_STRESS,
                    cast(PR.NPD as REAL) as NPD,
                    PL.FLUIDCODE as FLUIDCODE,
                    PL.SEQUENCENUMBER,
                    x8.HeatTracingMedium
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join JPipingSystem PS on PS.Oid = x6.OidDestination
                    join XSystemHierarchy x4 on x4.OidOrigin = PS.Oid
                    join JPipelineSystem PL on PL.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = PL.Oid
                    join XSystemHierarchy x1 on x1.OidOrigin = PL.Oid
                    join JRtePipeRun PR on PR.Oid = x1.OidDestination
                    join JNamedItem N2 on N2.Oid = PR.Oid
                    join JUADICPipeRun x2 on x2.oid = PR.oid
                    join JHeatTracingInfo x8 on x8.oid = PR.oid
                    order by PIPERUN
                    ", p_projectMasterID);

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                        dataTable.TableName = "SP3D_PIPERUN";
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            public static DataTable GetInstrumentAttributeDataSP3D(string p_host, string p_port, string p_service, string p_id, string p_password, string p_dbSchema, string p_projectMasterID)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Port = p_port,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {

                        string rQuery = string.Format(@"
                    select 
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(x2.oid as varchar(256)) as OID,
                    x2.ItemName as INSTRUMENT,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    x10.IndustryCommodityCode as INDUSTRYCOMMODITYCODE,
                    x2.ItemName as ITEMTAG
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
                    join XSystemHierarchy x4 on x4.OidOrigin = x7.OidDestination
                    join XSystemHierarchy x1 on x1.OidOrigin = x4.OidDestination
                    join JNamedItem N2 on N2.Oid = x1.OidDestination
                    join XOwnsParts x8 on x8.OidOrigin = x1.OidDestination
                    join JRteInstrument x9 on x9.Oid = x8.OidDestination
                    join JNamedItem x2 on x2.Oid = x9.Oid
                    join XmadeFrom x11 on x11.OidOrigin = x9.Oid
                    join JDPipeComponent x10 on x10.Oid = x11.OidDestination
                    UNION
                    select
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate, 
                    cast(x2.oid as varchar(256)) as OID,
                    x2.ItemName as INSTRUMENT,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    x10.IndustryCommodityCode as INDUSTRYCOMMODITYCODE,
                    x2.ItemName as ITEMTAG
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
                    join XSystemHierarchy x1 on x1.OidOrigin = x7.OidDestination
                    join JNamedItem N2 on N2.Oid = x1.OidDestination
                    join XOwnsParts x8 on x8.OidOrigin = x1.OidDestination
                    join JRteInstrument x9 on x9.Oid = x8.OidDestination
                    join JNamedItem x2 on x2.Oid = x9.Oid
                    join XmadeFrom x11 on x11.OidOrigin = x9.Oid
                    join JDPipeComponent x10 on x10.Oid = x11.OidDestination
                    UNION
                    select 
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(x2.oid as varchar(256)) as OID,
                    x2.ItemName as INSTRUMENT,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    x10.IndustryCommodityCode as INDUSTRYCOMMODITYCODE,
                    x2.ItemName as ITEMTAG
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join XSystemHierarchy x1 on x1.OidOrigin = x3.OidDestination
                    join JNamedItem N2 on N2.Oid = x1.OidDestination
                    join XOwnsParts x8 on x8.OidOrigin = x1.OidDestination
                    join JRteInstrument x9 on x9.Oid = x8.OidDestination
                    join JNamedItem x2 on x2.Oid = x9.Oid
                    join XmadeFrom x11 on x11.OidOrigin = x9.Oid
                    join JDPipeComponent x10 on x10.Oid = x11.OidDestination
                    UNION
                    select 
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(x2.oid as varchar(256)) as OID,
                    x2.ItemName as INSTRUMENT,
                    N2.itemname as PIPERUN,
                    N4.itemname as UNIT,
                    x10.IndustryCommodityCode as INDUSTRYCOMMODITYCODE,
                    x2.ItemName as ITEMTAG
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x1 on x1.OidOrigin = x5.OidDestination
                    join JNamedItem N2 on N2.Oid = x1.OidDestination
                    join XOwnsParts x8 on x8.OidOrigin = x1.OidDestination
                    join JRteInstrument x9 on x9.Oid = x8.OidDestination
                    join JNamedItem x2 on x2.Oid = x9.Oid
                    join XmadeFrom x11 on x11.OidOrigin = x9.Oid
                    join JDPipeComponent x10 on x10.Oid = x11.OidDestination
                    order by INSTRUMENT
                    ", p_projectMasterID);

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                        dataTable.TableName = "SP3D_INSTRUMENT";
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

            public static DataTable GetEquipmentAttributeDataSP3D(string p_host, string p_port, string p_service, string p_id, string p_password, string p_dbSchema, string p_projectMasterID)
            {
                DataTable dataTable = null;

                try
                {
                    using (AppOracleDatabase oracle = new AppOracleDatabase()
                    {
                        Host = p_host,
                        Service = p_service,
                        Id = p_id,
                        Password = p_password
                    })
                    {
                        string rQuery = string.Format(@"
                    select 
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(SE.oid as varchar(256)) as OID,
                    N.ItemName as EQUIPMENT,
                    N4.ItemName as UNIT,
                    N.ItemName as ITEMTAG,
                    EP.PartDescription as EQUIPMENTTYPE
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join XSystemHierarchy x7 on x7.OidOrigin = x3.OidDestination
                    join JMachinerySystem MS on MS.Oid = x7.OidDestination
                    join XHasEqpAsChild x4 on x4.OidOrigin = MS.Oid
                    join JSmartEquipmentOcc SE on SE.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = SE.Oid
                    join XSOtoSI_R SI on SI.OidOrigin = SE.Oid
                    join JDPart EP on EP.Oid = SI.OidDestination
                    UNION
                    select 
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(SE.oid as varchar(256)) as OID,
                    N.ItemName as EQUIPMENT,
                    N4.ItemName as UNIT,
                    N.ItemName as ITEMTAG,
                    EP.PartDescription as EQUIPMENTTYPE
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join XSystemHierarchy x3 on x3.OidOrigin = x5.OidDestination
                    join JMachinerySystem MS on MS.Oid = x3.OidDestination
                    join XHasEqpAsChild x4 on x4.OidOrigin = MS.Oid
                    join JSmartEquipmentOcc SE on SE.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = SE.Oid   
                    join XSOtoSI_R SI on SI.OidOrigin = SE.Oid
                    join JDPart EP on EP.Oid = SI.OidDestination   
                    UNION
                    select 
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(SE.oid as varchar(256)) as OID,
                    N.ItemName as EQUIPMENT,
                    N4.ItemName as UNIT,
                    N.ItemName as ITEMTAG,
                    EP.PartDescription as EQUIPMENTTYPE
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join XSystemHierarchy x5 on x5.OidOrigin = x6.OidDestination
                    join JMachinerySystem MS on MS.Oid = x5.OidDestination
                    join XHasEqpAsChild x4 on x4.OidOrigin = MS.Oid
                    join JSmartEquipmentOcc SE on SE.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = SE.Oid   
                    join XSOtoSI_R SI on SI.OidOrigin = SE.Oid
                    join JDPart EP on EP.Oid = SI.OidDestination
                    UNION
                    select
                    distinct
                    cast((SELECT {0} from DUAL) as int) as ProjectMasterID,
                    cast((SELECT 0 from DUAL) as int) as DeleteFlag,
                    (SELECT SYSDATE from DUAL) as ModifiedDate,
                    cast(SE.oid as varchar(256)) as OID,
                    N.ItemName as EQUIPMENT,
                    N4.ItemName as UNIT,
                    N.ItemName as ITEMTAG,
                    EP.PartDescription as EQUIPMENTTYPE
                    from JUnitSystem UN
                    join JNamedItem N4 on N4.Oid = UN.Oid
                    join XSystemHierarchy x6 on x6.OidOrigin = UN.Oid
                    join JMachinerySystem MS on MS.Oid = x6.OidDestination
                    join XHasEqpAsChild x4 on x4.OidOrigin = MS.Oid
                    join JSmartEquipmentOcc SE on SE.Oid = x4.OidDestination
                    join JNamedItem N on N.Oid = SE.Oid   
                    join XSOtoSI_R SI on SI.OidOrigin = SE.Oid
                    join JDPart EP on EP.Oid = SI.OidDestination
                    order by EQUIPMENT
                    ", p_projectMasterID);

                        dataTable = CreateCommand(rQuery, oracle, p_dbSchema);
                        dataTable.TableName = "SP3D_EQUIPMENT";
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message.ToString());
                }

                return dataTable;
            }

        }

        public class Common
        {
            public const string endPointAddress = @"http://172.22.118.97:8080/IDCSService.svc";
            //public const string endPointAddress = @"http://localhost:13371/IDCSService.svc";
            //public const string endPointAddressEDB = @"http://172.22.118.97:8080/EDBService.svc";
            //public const string endPointAddressSP3D = @"http://172.22.118.97:8080/SP3DModelService.svc";
        }


    }
}
