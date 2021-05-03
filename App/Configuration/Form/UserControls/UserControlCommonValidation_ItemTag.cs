using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

using Configuration.Properties;
using CommonLibrary;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using EDCS.WF.DSL.Entities;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using TechSun.Theme;
using Telerik.WinControls.UI;


namespace Configuration
{
    public partial class UserControlCommonValidation_ItemTag : UserControlBase
    {
        private enum menColumn { AttributeName, Separator, Length, FillString, Alignment, Duplication, IgnoreSeparator, Last };
        private string[] mColumnName = new string[] {"Property", "Separator", "Length", "Fill String", "Alignment", "Select one for duplication", "Ignore separator" };
        private int[] mColumnWidths = new int[] { 200, 100, 100, 100, 100, 120, 120 };
        private bool[] mColumnLock = new bool[] { true, true, true, true, true, false, false };

        string mProjectNo = string.Empty;
        string mItemType = string.Empty;
        private DataSet m_dsItemFormatXML = null;

        EntitySPTool_Project mSelectedProject = new EntitySPTool_Project();

        public UserControlCommonValidation_ItemTag()
        {
            InitializeComponent();
        }
        public UserControlCommonValidation_ItemTag(EntitySPTool_Project eSelectedProject, string sItemType)
        {
            InitializeComponent();

            mSelectedProject = eSelectedProject;
            mItemType = sItemType;
        }
        public override void LoadData()
        {
            //// ItemTagFormat.xml 경로 가져오기, ItemTagFormat.xml 출력
            if (mSelectedProject != null)
            {
                textBoxXMLPath.Text = GetItemTagFormatXMLPath(mSelectedProject);
            }

            //  spread 초기화 및 Tag ReadOnly Data 가져오기
            if (mSelectedProject != null)
                InitSpreadSheet(mSelectedProject);
        }

        string GetItemTagFormatXMLPath(EntitySPTool_Project eProject)
        {
            string sPath = "File Path : ";

            // (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
            Information information = new Information();
            information.Project = eProject;
            information.Plant = null;

            if (information.Project != null)
            {
                List<InfoPlant> lstPlant = new List<InfoPlant>();
                var proxy2 = WcfHelper.CreateProxyNetTcp<IEdcsDac>(true);
                using (proxy2 as IDisposable)
                {
                    lstPlant = proxy2.GetPlantNames(true, information, information.Project, GeneralUtil.InitConnection(information));
                }

                if (lstPlant.Count != 0)
                {
                    sPath = "File Path : " + lstPlant[0].SiteItemTagFormatXMLPath.ToString();
                }
            }

            return sPath;
        }

        /// <summary>
        /// UserControlSPPIDItemTagFormat 가져오기, xml file 출력
        /// </summary>
        /// <param name="eSelectedProject"></param>
        /// <param name="sItemType"></param>
        private void InitSpreadSheet(EntitySPTool_Project eSelectedProject)
        {
            string sLoadingTexts = "Loading data.  Please wait.";
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, EDCS.WF.BSL.Variable.SharedVariable.ToolTip);
            frmPopupWaitingBarCallMethod.MethodSetStart();
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

            try
            {
                //컬럼 세팅(셀타입, Visible, 정렬 등)
                setSpread();

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    // SPPID_TagNoSetting 테이블 조회
                    List<EntitySPPID_TagNoSetting> lstTagRule = proxy.ReadSPPID_MarkTagNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, eSelectedProject.ProjectID, mItemType, TagNoSetting.Validation);

                    /// spread setting & XML Load
                    if (m_dsItemFormatXML == null)
                    {
                        m_dsItemFormatXML = GetItemTagFormatSPPID();
                    }

                    /// XML에서 만든 DataTable로 spread에 출력
                    if (m_dsItemFormatXML.Tables.Contains(mItemType))
                    {
                        DataTable dtTable = m_dsItemFormatXML.Tables[mItemType];

                        fpSpreadItemTag.ActiveSheet.RowCount = dtTable.Rows.Count;

                        int iRowCount = 0;
                        foreach (DataRow dr in dtTable.Rows)
                        {
                            EntitySPPID_TagNoSetting tag = lstTagRule.Where(w => w.AttributeName == dr["Property"].ToString()).FirstOrDefault();
                            
                            // from ItemTagFormat.xml file
                            fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.AttributeName].Text = dr["Property"].ToString();
                            fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.Separator].Text = dr["Separator"].ToString();
                            fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.Length].Text = dr["Length"].ToString();
                            fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.FillString].Text = dr["FillString"].ToString();
                            fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.Alignment].Text = dr["Alignment"].ToString();

                            // from SPPID_TagNoSetting table
                            if (tag != null)
                            {
                                fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.Duplication].Value = tag.AllowDuplicationFlag;
                                fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.IgnoreSeparator].Value = tag.AllowNullFlag;
                            }
                            else
                            {
                                fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.Duplication].Value = false;
                                fpSpreadItemTag.ActiveSheet.Cells[iRowCount, (int)menColumn.IgnoreSeparator].Value = false;
                            }

                            iRowCount += 1;
                        }
                    }
                }

                // 컬럼 헤더 높이
                fpSpreadItemTag.ActiveSheet.ColumnHeader.Rows.Get(0).Height = 40F;

                frmPopupWaitingBarCallMethod.MethodClose();
            }
            catch (Exception ex)
            {
                frmPopupWaitingBarCallMethod.MethodClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }
        private void setSpread()
        {
            //컬럼 헤드 초기화
            InitSpread();

            // 컬럼 세팅
            for (int i = 0; i < fpSpreadItemTag.ActiveSheet.Columns.Count; i++)
            {
                switch (i)
                {
                    case (int)menColumn.Duplication:
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadItemTag.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadItemTag.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn.IgnoreSeparator:
                        CheckBoxCellType ctCheckBox1 = new CheckBoxCellType();
                        fpSpreadItemTag.ActiveSheet.Columns[i].CellType = ctCheckBox1;
                        fpSpreadItemTag.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    default:
                        TextCellType ctText = new TextCellType();
                        fpSpreadItemTag.ActiveSheet.Columns[i].CellType = ctText;
                        break;
                }
            }

        }
        public void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadItemTag, mColumnWidths, mColumnName, mColumnLock, null, null, null, null, null, Color.LightGray, true, true);
        }
        ///// <summary>
        ///// textBoxXMLPath에서 Split으로 주소만 가져옴
        ///// </summary>
        ///// <returns></returns>
        private string GetXMLpath()
        {
            string[] arXMLPath = textBoxXMLPath.Text.Split(":".ToCharArray());

            if (arXMLPath[1].Length > 1)
            {
                return arXMLPath[1].Trim();
            }

            return string.Empty;
        }

        ///// <summary>
        ///// ItemTag 'Validation'Type Data Setting 저장
        ///// </summary>
        public override void SaveData()
        {
            bool bResult = false;
            string sMessage = string.Empty;

            DataTable dtItemTagSetting = new DataTable("Table1");

            dtItemTagSetting.Columns.Add("TagID", typeof(long));
            dtItemTagSetting.Columns.Add("ItemType");
            dtItemTagSetting.Columns.Add("Type");
            dtItemTagSetting.Columns.Add("ProjectID", typeof(long));
            dtItemTagSetting.Columns.Add("AttributeName");
            dtItemTagSetting.Columns.Add("Separator");
            dtItemTagSetting.Columns.Add("Length", typeof(int));
            dtItemTagSetting.Columns.Add("FillString");
            dtItemTagSetting.Columns.Add("Alignment");
            dtItemTagSetting.Columns.Add("AllowNullFlag", typeof(bool));
            dtItemTagSetting.Columns.Add("AllowDuplicationFlag", typeof(bool));
            dtItemTagSetting.Columns.Add("SortOrder", typeof(int));

            bool bSaveFlag = true;

            int iSortOrder = 0;
            for (int i = 0; i < fpSpreadItemTag.ActiveSheet.RowCount; i++)
            {
                DataRow drNew = dtItemTagSetting.NewRow();

                string sProperty = fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.AttributeName].Text;

                if (string.IsNullOrEmpty(sProperty))
                {
                    bSaveFlag = false;
                    break;
                }

                drNew["ItemType"] = mItemType;
                drNew["Type"] = TagNoSetting.Validation;
                drNew["ProjectID"] = mSelectedProject.ProjectID;
                drNew["AttributeName"] = fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.AttributeName].Value.ToString();
                drNew["Separator"] = fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.Separator].Text.ToString();

                if (!string.IsNullOrEmpty(fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.Length].Text.Trim()))
                    drNew["Length"] = int.Parse(fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.Length].Text.ToString());

                drNew["FillString"] = fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.FillString].Text.ToString();
                drNew["Alignment"] = fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.Alignment].Text.ToString();

                if (fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.Duplication].Text.ToUpper().Equals("TRUE"))
                    drNew["AllowDuplicationFlag"] = true;
                else
                    drNew["AllowDuplicationFlag"] = false;

                if (fpSpreadItemTag.ActiveSheet.Cells[i, (int)menColumn.IgnoreSeparator].Text.ToUpper().Equals("TRUE"))
                    drNew["AllowNullFlag"] = true;
                else
                    drNew["AllowNullFlag"] = false;


                drNew["SortOrder"] = iSortOrder;

                dtItemTagSetting.Rows.Add(drNew);
                iSortOrder++;
            }

            if (bSaveFlag)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    bResult = proxy.SaveSPPID_TagNoSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mSelectedProject.ProjectID, mItemType, TagNoSetting.Validation, dtItemTagSetting, ref sMessage);
                }
            }
            else
            {
                sMessage = string.Format("{0} : 'Property' is empty", TagNoSetting.Validation);
            }

            // 결과 메세지 출력
            if(bResult != false)
                new frmInfomation(Resources.Message10000).ShowDialog(this);
            else
                new frmWarning(sMessage).ShowDialog(this);
        }

        /// <summary>
        /// XML 분석해 DataSet에 담기
        /// </summary>
        /// <returns></returns>
        public DataSet GetItemTagFormatSPPID()
        {
            DataSet dsItemTagConfiguration = new DataSet("dsItemTagConfiguration");

            try
            {
                // ItemTagFormat.xml 에서 데이터를 조회 한다. 
                if (!string.IsNullOrEmpty(GetXMLpath()))
                {

                    XmlDocument xmlDoc = new XmlDocument();
                    xmlDoc.Load(GetXMLpath());

                    foreach (XmlNode node in xmlDoc.ChildNodes[0])
                    {
                        #region # 이전 구문 혹시 에러시 참조 하기 위해 주석으로 남겨 놓음
                        //string sItemType = node.Attributes[0].Value;
                        //string sItemTagFormat = node.ChildNodes[0].Attributes[0].Value;
                        //string sFixedLength = node.ChildNodes[0].Attributes[2].Value;
                        //string sFillingString = node.ChildNodes[0].Attributes[3].Value;
                        #endregion

                        string sItemType = node.Attributes["ITEMTYPE"].Value;

                        XmlNode childNodes = null;

                        if (node.ChildNodes.Count > 0)
                        {
                            childNodes = node.ChildNodes[0];
                        }

                        string sItemTagFormat = string.Empty;
                        string sFixedLength = string.Empty;
                        string sFillingString = string.Empty;
                        string sAlignment = string.Empty;

                        List<string> lstProperty = new List<string>();
                        List<string> lstSeparator = new List<string>();
                        List<string> lstFillingString = new List<string>();
                        List<string> lstFixedlength = new List<string>();
                        List<string> lstAlignment = new List<string>();

                        foreach (XmlAttribute attribute in childNodes.Attributes)
                        {
                            switch (attribute.Name)
                            {
                                case "ItemTagFormat":
                                    sItemTagFormat = attribute.Value;
                                    // (TagPrefix)-(Unit...UnitCode)(TagSequenceNo)(TagSuffix)
                                    // Property
                                    SetFillAttributeList(sItemTagFormat, lstProperty);

                                    // Separator
                                    foreach (string sTemp in sItemTagFormat.Split(')'))
                                    {
                                        if (sTemp.Contains("("))
                                        {
                                            string sSeparatorName = sTemp.Substring(0, sTemp.IndexOf("(")).Trim();
                                            lstSeparator.Add(sSeparatorName);
                                        }
                                    }

                                    break;
                                case "FixedLength":
                                    sFixedLength = attribute.Value;
                                    // ()()(2)()
                                    SetFillAttributeList(sFixedLength, lstFixedlength);

                                    break;
                                case "FillingString":
                                    sFillingString = attribute.Value;
                                    // ()()(0)()
                                    SetFillAttributeList(sFillingString, lstFillingString);
                                    break;
                                case "Alignment":
                                    sAlignment = attribute.Value;
                                    // (Left)(Left)(Right)()
                                    SetFillAttributeList(sAlignment, lstAlignment);
                                    break;

                            }
                        }

                        DataTable dtItemtype = new DataTable(sItemType);

                        dtItemtype.Columns.Add("Property");         // ItemTagFormat
                        dtItemtype.Columns.Add("Separator");        // ItemTagFormat
                        dtItemtype.Columns.Add("Length");           // FixedLength
                        dtItemtype.Columns.Add("FillString");       // FillingString
                        dtItemtype.Columns.Add("Alignment");        // Alignment

                        int i = 0;
                        foreach (string sProperty in lstProperty)
                        {
                            #region # 데이터 체우기
                            DataRow drNew = dtItemtype.NewRow();
                            drNew["Property"] = lstProperty[i];

                            if (lstSeparator.Count > i)
                            {
                                drNew["Separator"] = lstSeparator[i];
                            }
                            else
                            {
                                drNew["Separator"] = string.Empty;
                            }

                            if (lstFillingString.Count > i)
                            {
                                drNew["FillString"] = lstFillingString[i];
                            }
                            else
                            {
                                drNew["FillString"] = string.Empty;
                            }

                            if (lstFixedlength.Count > i)
                            {
                                drNew["Length"] = lstFixedlength[i];
                            }
                            else
                            {
                                drNew["Length"] = string.Empty;
                            }

                            if (lstAlignment.Count > i)
                            {
                                drNew["Alignment"] = lstAlignment[i];
                            }
                            else
                            {
                                drNew["Alignment"] = string.Empty;
                            }

                            dtItemtype.Rows.Add(drNew);

                            i++;
                            #endregion
                        }

                        dsItemTagConfiguration.Tables.Add(dtItemtype);
                    }
                }
            }
            catch (Exception ex)
            {
            }

            return dsItemTagConfiguration;
        }

        /// <summary>
        /// lstProperty 에 ItemTag를 구성하는 Property 각각 담기
        /// </summary>
        /// <param name="sAttributeValue"></param>
        /// <param name="lst"></param>
        private void SetFillAttributeList(string sAttributeValue, List<string> lst)
        {
            foreach (string sTemp in sAttributeValue.Split('('))
            {
                if (sTemp.Contains(")"))
                {
                    string sAttributeName = sTemp.Substring(0, sTemp.IndexOf(")")).Trim();
                    lst.Add(sAttributeName);
                }
            }
        }

    }
}
