using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Data;
using System.Windows.Forms;
using Configuration.Properties;
using ScintillaNET;
using ScriptingInterface;
using Telerik.WinControls.UI;

using IDCS.Framework.Service;
using IDCS.Service.Interface;
using IDCS.DataTransferObjects;

using EDCS.WF.DSL;

using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlSP3DLineNoDisplayFormat : UserControlBase
    {
        public string DisplayName { get; set; }
        public string Value { get; set; }


        private List<UserControlRevSetting.IssueDateDisplayFormat> IssueDateDisplayFormatList;

        public UserControlSP3DLineNoDisplayFormat()
        {
            InitializeComponent();
            {
                this.scintillaEdit = new Scintilla();
                // 
                // scintillaEdit
                // 
                this.scintillaEdit.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
                //    ((AnchorStyles)((((AnchorStyles.Top | AnchorStyles.Bottom)
                //| AnchorStyles.Left)
                //| AnchorStyles.Right)));
                this.scintillaEdit.Location = new Point(15, 24);
                this.scintillaEdit.Name = "scintillaEdit";
                this.scintillaEdit.Dock = DockStyle.Fill;
                this.scintillaEdit.Size = new Size(436, 153);

                this.scintillaEdit.Styles.BraceBad.Size = 9F;
                this.scintillaEdit.Styles.BraceLight.Size = 9F;
                this.scintillaEdit.Styles.ControlChar.Size = 9F;
                this.scintillaEdit.Styles.Default.BackColor = SystemColors.Window;
                this.scintillaEdit.Styles.Default.Size = 9F;
                this.scintillaEdit.Styles.IndentGuide.Size = 9F;
                this.scintillaEdit.Styles.LastPredefined.Size = 9F;
                this.scintillaEdit.Styles.LineNumber.Size = 9F;
                this.scintillaEdit.Styles.Max.Size = 9F;
                this.scintillaEdit.TabIndex = 10;

                this.radGroupBox1.Controls.Add(this.scintillaEdit);


            }

        }

        public override void LoadData()
        {
            this.radDropDownListProject.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
            this.CallMethodSetting();
            this.RegisterEventHandler();

            long iProjectMasterID = ((EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem).ProjectMasterID;
            this.LoadData_Service(iProjectMasterID.ToString());
        }

        private void RegisterEventHandler()
        {
            this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
        }

        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                this.scintillaEdit.Text = string.Empty;
                this.radTextBoxResult.Text = string.Empty;
                this.radTextBoxLineNo.Text = string.Empty;

                if (radDropDownListProject.SelectedItem.DataBoundItem != null)
                {
                    long iProjectMasterID = ((EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem).ProjectMasterID;
                    this.LoadData_Service(iProjectMasterID.ToString());


                }
            }
            catch (System.ServiceModel.FaultException ex)
            {

                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {

                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }


        }

        private void CallMethodSetting()
        {
            //this.MethodBindingProjectDropDownList();
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
        }

        private void MethodBindingProjectDropDownList()
        {
            try
            {
                using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(AppDocData.ServiceURL))
                {
                    List<ProjectDTO> oProjectList = new List<ProjectDTO>();
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
                    foreach (ProjectDTO oPrjDTO in oProjectList)
                    {
                        if (!oPrjDTO.Enabled) continue;
                        RadListDataItem oItem = new RadListDataItem();
                        {
                            oItem.Text = string.Format("{0}({1})", oPrjDTO.Name, oPrjDTO.Desc);
                            oItem.Tag = oPrjDTO;
                        }
                        radDropDownListProject.Items.Add(oItem);
                    }

                    if (radDropDownListProject.Items.Count > 0)
                    {
                        radDropDownListProject.SelectedIndex = 0;
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog();
            }
        }

        private void LoadData_Service(string sProjectID)
        {
            {
                DataSet ds = SvcProxy.GetSp3dConfig(sProjectID, "ItemCompare", "LineNo");
                if (ds.Tables[0] == null || ds.Tables[0].Rows.Count == 0)
                {
                    ds = SvcProxy.GetSp3dConfig(sProjectID, "ItemCompareDefault", "LineNo");
                }

                string sScript = ds.Tables[0].Rows[0]["VALUE"].ToString();
                this.scintillaEdit.Text = sScript;

                ds = SvcProxy.GetSp3dConfig(sProjectID, "ItemCompare", "LineNoSample");
                if (ds.Tables[0].Rows.Count > 0)
                {
                    string sSampleLineNo = ds.Tables[0].Rows[0]["VALUE"].ToString();
                    this.radTextBoxLineNo.Text = sSampleLineNo;
                }
            }

            radPageViewPage1.Controls.Clear();
            radPageViewPage2.Controls.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var spProject = proxy.ReadSPTool_ProjectList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, long.Parse(sProjectID), ToolType.SPPID);
                if (spProject != null)
                {
                    this.InitTagSetting(spProject, Information.PIPERUN);
                }
            }

        }

        private void UserControlSP3DLineNoDisplayFormat_Load(object sender, System.EventArgs e)
        {
            this.scintillaEdit.ConfigurationManager.Language = "cs";
            this.scintillaEdit.ConfigurationManager.Configure();
        }

        private void radButtonOK_Click(object sender, EventArgs e)
        {
            this.SaveData();
        }

        public override void SaveData()
        {
            long lProjectMasterID = ((EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem).ProjectMasterID;

            //string sEndPointAddress = AppDocData.ServiceURL + "IDCSService.svc";
            //using (IIDCSService svcProxy = ServiceFactory.CreateServiceChannel<IIDCSService>(sEndPointAddress))
            {
                List<ProjectConfigDTO> configs = new List<ProjectConfigDTO>();
                try
                {
                    configs.Add(new ProjectConfigDTO() { Section = "ItemCompare", Key = "LineNo", Value = this.scintillaEdit.Text });
                    configs.Add(new ProjectConfigDTO() { Section = "ItemCompare", Key = "LineNoSample", Value = this.radTextBoxLineNo.Text });
                    SvcProxy.SaveSp3dConfigs(configs, lProjectMasterID.ToString());

                    MessageBox.Show("successfully!", "Save");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }
        }

        private void InitTagSetting(EntitySPTool_Project cpProject, string sItemType)
        {
            UserControlSPPIDMarkTagNo tagRule = new UserControlSPPIDMarkTagNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectNo, TagNoSetting.TagNo3D, cpProject.ProjectID, sItemType, true);
            tagRule.Dock = DockStyle.Fill;
            radPageViewPage1.Controls.Add(tagRule);

            UserControlSPPIDMarkTagNo tagRule2 = new UserControlSPPIDMarkTagNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectNo, TagNoSetting.TagNo3D_PipeRun, cpProject.ProjectID, sItemType, true);
            tagRule2.Dock = DockStyle.Fill;
            radPageViewPage2.Controls.Add(tagRule2);
        }

        private void radButtonEvaluate_Click(object sender, EventArgs e)
        {
            string sScriptCode =
@"using System;
using System.Linq;
using System.Collections.Generic;
public class Evaluator
{
	public static string ConvertToLineNo(string sLineNo)
	{"
	+ this.scintillaEdit.Text +
    @"}
}";
            try
            {
                Cursor.Current = Cursors.WaitCursor;

                ScriptEngine oScriptEngine = new ScriptEngine(sScriptCode);
                this.radTextBoxResult.Text = oScriptEngine.ConvertToLineNo(this.radTextBoxLineNo.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                Cursor.Current = Cursors.Default;
            }
        }
    }
}
