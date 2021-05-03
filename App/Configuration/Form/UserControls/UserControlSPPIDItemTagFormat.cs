using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using System.Text;
using Configuration.Properties;
using CommonLibrary;
using System.Xml;

namespace Configuration
{
    public partial class UserControlSPPIDItemTagFormat : UserControl
    {
        private enum LTColumn { LTProperty, LTSeparator, LTLength, LTFillString, LTAlignment, LTLast };
        private string[] m_arColumnNameItemTag = { "Property", "Separator", "Length", "Fill String", "Alignment" };
        private int[] m_arColumnWidthItemTag = new int[] { 200, 100, 100, 100, 100 };
        private bool[] marColumnLock = new bool[] { true, true, true, true, true };

        string mProjectNo = string.Empty;
        string mType = string.Empty;

        long mProjectID = -1;
        string mItemType = string.Empty;

        private DataSet m_dsItemFormatXML = null;
        private string m_SiteItemTagFormatXMLPath = string.Empty;
        
        public UserControlSPPIDItemTagFormat()
        {
            InitializeComponent();
        }

        public UserControlSPPIDItemTagFormat(bool isIE, string sProjectNo, string sSettingType, long lProjectID, string sItemType, string XMLPath)
        {
            InitializeComponent();

            mProjectNo = sProjectNo;
            mProjectID = lProjectID;
            mItemType = sItemType;
            mType = sSettingType;
            m_SiteItemTagFormatXMLPath = XMLPath;

            SetItemTagData();
        }
        
        /// <summary>
        /// spread setting & XML Load
        /// </summary>
        private void SetItemTagData()
        {
            initSpread();

            if (m_dsItemFormatXML == null)
            {
                m_dsItemFormatXML = GetItemTagFormatSPPID();
            }

            SetItemTagSpread();
        }

        private void initSpread()
        {
            SpreadUtil.InitSpread(fpSpreadTagFormat, m_arColumnWidthItemTag, m_arColumnNameItemTag, marColumnLock, null, null, null, null, null, Color.LightGray, true, true);
        }
        /// <summary>
        /// fpSpreadTagFormat에 로드된 ItemType만 복사
        /// </summary>
        private void SetItemTagSpread()
        {
            DataTable dtTable = m_dsItemFormatXML.Tables[mItemType];
            if (dtTable != null)
            {
                DataRow[] dtRows = dtTable.Select();

                fpSpreadTagFormat.ActiveSheet.RowCount = dtRows.Length;

                if (fpSpreadTagFormat.ActiveSheet.RowCount == 0)
                    return;

                fpSpreadTagFormat.DataSource = dtRows.CopyToDataTable();
                SpreadUtil.SetColumnWidth(fpSpreadTagFormat.ActiveSheet, m_arColumnWidthItemTag);
            }
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
                if (!string.IsNullOrEmpty(m_SiteItemTagFormatXMLPath))
                {
                    
                    XmlDocument xmlDoc = new XmlDocument();
                    xmlDoc.Load(m_SiteItemTagFormatXMLPath);

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
                //RadFormProgressForm.FormClose();
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
