using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using Configuration.Properties;
using CommonUtil.CommonLib;
using EDCS.WF.DSL;
using Telerik.WinControls.Enumerations;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlPBS : UserControl
    {
        

        private Information mInformation = Information.GetInstance();
        public bool IsVisibleBookMarkIcon = true;
       
        public const int CONST_SITE_PLANT_TYPE = 69;
        public const int CONST_PLANT_PLANT_TYPE = 70;
        public const int CONST_AREA_PLANT_TYPE = 72;
        public const int CONST_UNIT_PLANT_TYPE = 65;
        public const int CONST_DRAWING_PLANT_TYPE = 0;

        public UserControlPBS()
        {
            InitializeComponent();
        }

        private void UserControlPBS_Load(object sender, EventArgs e)
        {
            ClearPBSData();
        }
   
        public void ClearPBSData()
        {
            radTreeViewPBS.Nodes.Clear();
            SetDrawingCount();                
        }


        public void SetPBSData(bool isIE)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Loading data.  Please wait!";

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                try
                {
                    pictureBoxBookMark.Visible = IsVisibleBookMarkIcon;
                    pictureBoxBookMarkAdd.Visible = IsVisibleBookMarkIcon;

                    string sError = string.Empty;

                    DataTable dtPBS = proxy.GetPBSData(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation);
                    RadTreeNode node = new RadTreeNode();

                    radTreeViewPBS.Nodes.Clear();
                    radTreeViewPBS.Font = Information.FONT_REGULAR;
                    if (dtPBS.Rows.Count > 0)
                    {
                        foreach (DataRow dataRow in dtPBS.Rows)
                        {
                            node = new RadTreeNode();
                            node.Font = Information.FONT_REGULAR;
                            node.Name = dataRow["SP_ID"].ToString();

                            if (string.IsNullOrEmpty(Util.NullToSpace(dataRow["SP_DRAWINGID"])) == false)
                            {
                                node.Text = dataRow["SP_DRAWINGID"].ToString();
                            }
                            else
                            {
                                node.Text = dataRow["NAME"].ToString();
                            }

                            node.Tag = new ArrayList() { dataRow["PLANTGROUPTYPE"].ToString(), node.Text };
                            RadTreeNode nodParent = radTreeViewPBS.GetNodeByName(dataRow["PARENTID"].ToString());

                            if (nodParent == null)
                            {
                                node.Image = Resources.SmallPlant;
                                radTreeViewPBS.Nodes.Add(node);
                            }
                            else
                            {
                                nodParent.Nodes.Add(node);
                            }
                        }
                    }

                    radTreeViewPBS.ExpandAll();
                    radTreeViewPBS.SortOrder = SortOrder.Ascending;
                    radCheckBoxExpandAll.ToggleState = ToggleState.On;

                    //SetDefaultBookMark();
                    SetDrawingCount();
                    FrmProgressStatus.FormClose();
                }
                catch (System.ServiceModel.FaultException ex)
                {
                    FrmProgressStatus.FormClose();
                    
                    new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
                }
            }
        }

        private void SetDrawingCount()
        {
            int iTotalCount = 0;
            int iSelCount = 0;

            FindDrawingNodeCount(radTreeViewPBS.Nodes, false, ref iTotalCount);
            FindDrawingNodeCount(radTreeViewPBS.Nodes, true, ref iSelCount);
            radLabelTotalCount.Text = string.Format( "Total : {0},   Selected : {1}", iTotalCount, iSelCount);            
        }

        public bool IsAllChecked()
        {
            int iTotalCount = 0;
            int iSelCount = 0;

            FindDrawingNodeCount(radTreeViewPBS.Nodes, false, ref iTotalCount);
            FindDrawingNodeCount(radTreeViewPBS.Nodes, true, ref iSelCount);

            if (iTotalCount == iSelCount)
                return true;

            return false;
        }

        private void FindDrawingNodeCount(RadTreeNodeCollection nodes, bool bCheckOnly, ref int count)
        {
            foreach (RadTreeNode node in nodes)
            {
                ArrayList arList = (ArrayList)node.Tag;
                int nplantGroupType = Convert.ToInt32(arList[0]);

                if (nplantGroupType == 0)
                {
                    if (bCheckOnly == false)
                        count++;
                    else if (bCheckOnly == true && node.Checked == true)
                        count++;
                }


                if (node.Nodes.Count > 0) { FindDrawingNodeCount(node.Nodes, bCheckOnly, ref count); }
            }
        }

        private void radTreeViewPBS_NodeCheckedChanged(object sender, TreeNodeCheckedEventArgs e)
        {
            if (e.Node.Checked == true)
            {
                foreach (RadTreeNode node in e.Node.Nodes)
                {
                    node.Checked = true;
                }
            }
            else
            {
                foreach (RadTreeNode node in e.Node.Nodes)
                {
                    node.Checked = false;
                }
            }

            SetDrawingCount();
        }

        private void radCheckBoxExpandAll_ToggleStateChanged(object sender, StateChangedEventArgs args)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;

            if (radCheckBoxExpandAll.Checked)
            {
                FrmProgressStatus.ProgressPer = "Expanding PlantStructure. ";
                ExpandAll();
            }
            else
            {
                FrmProgressStatus.ProgressPer = "Collapsing PlantStructure. ";
                CollapseAll();
            }

            FrmProgressStatus.FormClose();
        }


        private void ExpandAll()
        {            
            if (!radCheckBoxExpandAll.Checked)
            {
                radCheckBoxExpandAll.Checked = true;
            }

            radTreeViewPBS.ExpandAll();
        }

        private void CollapseAll()
        {
            if (radCheckBoxExpandAll.Checked)
            {
                radCheckBoxExpandAll.Checked = false;
            }

            radTreeViewPBS.CollapseAll();
            radTreeViewPBS.Nodes[0].Expand();
        }

        public void SetCheckedList(List<string> LstDwg)
        {
            RadTreeNode[] node;           
            ResetChecked();
              
            List<string> lstAll = new List<string>();

            foreach (string sDwg in LstDwg)
            {                  
                node = radTreeViewPBS.FindNodes(sDwg);

                if (node != null && node.Length > 0)
                    node[0].Checked = true;                                   
            }          
        }

        public void ResetChecked()
        {
            foreach (RadTreeNode node in radTreeViewPBS.CheckedNodes)
            {
                node.Checked = false;
            }
        }

        public List<string> GetCheckedList()
        {
            List<string> lstDwg = new List<string>();

            foreach (RadTreeNode node in radTreeViewPBS.CheckedNodes)
            {
                ArrayList arTag = (ArrayList)node.Tag;
                int plantGroupType = Convert.ToInt32(arTag[0]);
                if (plantGroupType == 0)
                {
                    string sDrawingNo = arTag[1].ToString(); 
                    lstDwg.Add(sDrawingNo);
                }
            }

            return lstDwg;
        }

        public List<string> GetDrawingList()
        {
            List<string> lstDwg = new List<string>();

            foreach (RadTreeNode node in radTreeViewPBS.Nodes)
            {
                GetDrawingList(node, ref lstDwg);
            }

            return lstDwg;
        }

        private void GetDrawingList(RadTreeNode tn, ref List<string> lstDwg)
        {
            foreach (RadTreeNode child in tn.Nodes)
            {
                GetDrawingList(child, ref lstDwg);

                ArrayList arTag = (ArrayList)child.Tag;
                int plantGroupType = Convert.ToInt32(arTag[0]);
                if (plantGroupType == 0)
                {
                    string sDrawingNo = arTag[1].ToString();
                    lstDwg.Add(sDrawingNo);
                }
            }
        }

        public void GetUnits(ArrayList nodeList, RadTreeNode node)
        {
            if (node == null)
                node = radTreeViewPBS.Nodes[0];

            ArrayList arTag = (ArrayList)node.Tag;
            int plantGroupType = Convert.ToInt32(arTag[0]);
            switch (plantGroupType)
            {
                case CONST_SITE_PLANT_TYPE:
                case CONST_PLANT_PLANT_TYPE:
                case CONST_AREA_PLANT_TYPE:
                    for (int i = 0; i < node.Nodes.Count; i++)
                    {
                        GetUnits(nodeList, node.Nodes[i]);
                    }
                    break;
                case CONST_UNIT_PLANT_TYPE:
                    nodeList.Add(node.Text);
                    break;
            }
        }

    }
}
