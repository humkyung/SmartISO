using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls.UI;

using EDCS.WF.DSL;
using TechSun.Theme;
using Configuration.Properties;
using EDCS.WF.BSL;

namespace Configuration
{
    public partial class FrmCommonValidation_FilterConditionSetting : KRadForm
    {
        
        public Information mInformation = Information.GetInstance();
        EntitySPTool_Project mSelectedProject = new EntitySPTool_Project();
        DataTable mdtFilterCondition = new DataTable();
        string mItemType = string.Empty;

        public string m_sFilter;
        public string m_sComment;
        public bool m_bSkip;

        public FrmCommonValidation_FilterConditionSetting()
        {
            InitializeComponent();
        }

        public FrmCommonValidation_FilterConditionSetting(EntitySPTool_Project eSelectedProject, string sItemType)
        {
            InitializeComponent();
            mSelectedProject = eSelectedProject;
            mItemType = sItemType;

            // init combobox 
            userControlFilter1.SetCboColumns(eSelectedProject.ProjectID, sItemType);
            
            // init plant list
            SetPlantInfo();
        }
        public FrmCommonValidation_FilterConditionSetting(EntitySPTool_Project eSelectedProject, string sItemType, DataTable dtFilterCondition)
        {
            InitializeComponent();
            mSelectedProject = eSelectedProject;
            mItemType = sItemType;
            mdtFilterCondition = dtFilterCondition;

            // filter, comment, skip 등을 가져온 data로 채운다
            SetControls(dtFilterCondition);

            // initialize combobox 
            userControlFilter1.SetCboColumns(eSelectedProject.ProjectID, sItemType);

            // initialize plant list
            SetPlantInfo();
        }

        private void SetPlantInfo()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bool bGetDBPath = false;

                mInformation.Project = mSelectedProject;
                mInformation.Plant = null;

                List<InfoPlant> lstPlant = proxy.GetPlantNames(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, mSelectedProject, GeneralUtil.InitConnection(mInformation));

                foreach (InfoPlant data in lstPlant)
                {
                    if(bGetDBPath == false)
                    {
                        mInformation.Plant = data;
                        mInformation.InfoDB = new InfoDB();
                        mInformation.InfoDB.DBPath = Path.Combine(Paths.PATH_DATA, mInformation.Project.ProjectNo, mInformation.Plant.SiteName);
                        bGetDBPath = true;
                    }

                    RadListDataItem item = new RadListDataItem(data.SiteName);
                    item.Tag = data;
                    radDropDownListPlant.Items.Add(item);
                }


                if (radDropDownListPlant.Items.Count > 0)
                    radDropDownListPlant.Items[0].Selected = true;
            }
        }

        /// <summary>
        /// filter, comment, skip 등을 가져온 data로 채운다
        /// </summary>
        /// <param name="dtFilterCondition"></param>
        private void SetControls(DataTable dtFilterCondition)
        {
            // initialize Filter Text
            userControlFilter1.SetFilters(dtFilterCondition.Rows[0]["FIlter"].ToString());

            // initialize CheckBox
            if (dtFilterCondition.Rows[0]["SkipErrorCheckFlag"].ToString().ToUpper().Equals("TRUE"))
                radCheckBoxSkipErrorCheck.Checked = true;
            else
                radCheckBoxSkipErrorCheck.Checked = false;

            // initialize Comment Text
            radTextBoxComment.Text = dtFilterCondition.Rows[0]["Comment"].ToString();
        }

        private void kRadButtonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }
        /// <summary>
        /// 생성된 Filter condition가 적용된 plant data를 미리보기 한다
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void kRadButtonExcute_Click(object sender, EventArgs e)
        {
            m_sFilter = userControlFilter1.GetFilters();

            if (string.IsNullOrEmpty(m_sFilter))
            {
                new frmInfomation("Query string is empty.").ShowDialog(this);
            }
            else
            {
                // 선택된 Itemtype의 전체 Data가져오기
                DataTable dtItemType = new DataTable(mItemType);
                string sItemDBType = Information.GetItemDBType(mItemType);

                //EdcsDac의 로직에 lstSelectedDrawing(10), lstTotalDrawing(10)  갯수가 같으면 전체 조회하고 있음
                List<string> lstSelectedDrawing = new List<string> { "Dummy_1", "Dummy_2" };

                List<EntitySPPID_PipingCompSetting> lstPipingCompSetting = new List<EntitySPPID_PipingCompSetting>();


                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    dtItemType = proxy.GetItemTypeData_Byte(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sItemDBType, mItemType, mInformation.InfoDB.DBPath, lstPipingCompSetting, lstSelectedDrawing, lstSelectedDrawing, string.Empty, false);
                }

                if (dtItemType.Rows.Count == 0)
                {
                    new frmInfomation("Cannot find Database.").ShowDialog(this);
                }
                else
                {
                    //사용자가 설정한 Filter condition text에서 Column List를 추출/작성
                    List<string> lstColumnName = new List<string>() { "ITEM_SPID" };
                    m_sFilter = m_sFilter.Replace("\r\n", " ");
                    string[] arTemp = m_sFilter.Split(new string[] { "<>", ">", "<", ">=", "<=", "=", "LIKE", "IS NULL", "IS NOT NULL", " AND ", " OR ", " and ", " or ", "like", "is null", "is not null" }, StringSplitOptions.RemoveEmptyEntries);
                    for (int i = 0; i < arTemp.Length - 1; i += 2)
                    {
                        // 공백와 괄호를 제거(등호는 split에서 제거됨)
                        arTemp[i] = arTemp[i].Trim();
                        arTemp[i] = arTemp[i].Replace("(", "");
                        arTemp[i] = arTemp[i].Replace(")", "");

                        // 공백이 포함된 Attribute name 편집
                        if (arTemp[i].Contains(" "))
                        {
                            arTemp[i] = arTemp[i].Substring(arTemp[i].LastIndexOf(" ") + 1);
                        }

                        // Item_SPID가 포함된지 확인해 없으면 추가
                        bool IsExist = false;

                        foreach (string sColumnName in lstColumnName)
                        {
                            if (sColumnName.ToUpper() == arTemp[i].Trim().ToUpper())
                            {
                                IsExist = true;
                                break;
                            }
                        }

                        if (!IsExist)
                            lstColumnName.Add(arTemp[i]);
                    }

                    //RadFormProgressForm.FormShow();
                    //RadFormProgressForm.SetMarquee();
                    //RadFormProgressForm.ProgressPer = "Executing query.  Please wait!";

                    try
                    {
                        DataRow[] drResult = dtItemType.Select(m_sFilter);

                        if (drResult.Length == 0)
                        {
                            //RadFormProgressForm.FormClose();
                            new frmInfomation(string.Format(Resources.Message10023, "result")).ShowDialog(this);
                            return;
                        }

                        DataTable dtResult = drResult.CopyToDataTable();
                        DataView dv = new DataView(dtResult);

                        dtResult = dv.ToTable(false, lstColumnName.ToArray());

                        fpSpreadQueryResult.Reset();
                        fpSpreadQueryResult.DataSource = dtResult;

                        // Column Lock 설정 및 width array 초기화
                        int[] arColumnWidthQueryResult = new int[fpSpreadQueryResult.ActiveSheet.Columns.Count];
                        foreach (Column col in fpSpreadQueryResult.ActiveSheet.Columns)
                        {
                            col.Locked = true;
                            
                            arColumnWidthQueryResult[col.Index] = 100;
                        }

                        SpreadUtil.SetAllColumnAutoFilterAndSort(fpSpreadQueryResult.ActiveSheet);
                        SpreadUtil.SetColumnWidth(fpSpreadQueryResult.ActiveSheet, arColumnWidthQueryResult);
                        //RadFormProgressForm.FormClose();

                    }
                    catch (Exception ex)
                    {
                        //RadFormProgressForm.FormClose();
                        new frmWarning(ex.Message).ShowDialog(this);
                    }
                }
            }



            


        }

        private void kRadButtonOK_Click(object sender, EventArgs e)
        {
            // OK 클릭시 체크항목 : SPPID_FilterCondition 컬럼별 not null상태, color는 empty
            //
            //
            if (string.IsNullOrEmpty(userControlFilter1.GetFilters()))
            {
                MessageBox.Show("Please enter your information in the Filter.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            else
                m_sFilter = userControlFilter1.GetFilters();

            if (radTextBoxComment.Text == string.Empty || radTextBoxComment.Text == null)
            {
                MessageBox.Show("Please enter your information in the Comment.", this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            else
                m_sComment = radTextBoxComment.Text;


            m_bSkip = radCheckBoxSkipErrorCheck.Checked;
            this.DialogResult = System.Windows.Forms.DialogResult.OK;

        }
    }
}
