using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms;

namespace Configuration
{
    //[Designer("System.Windows.Forms.Design.ParentControlDesigner, System.Design", typeof(System.ComponentModel.Design.IDesigner))]
    public partial class UserControlComboTreeBox : UserControl
    {
        public delegate void TextChangingHandler(object sender, EventArgs e);
        public event TextChangingHandler EditChanging;
        public event TextChangingHandler SelectedChanged;

        const int CONST_HEIGHT = 21;
        private bool isDropDown = false;
        private int mControlHeight;
        private int mTreeViewHeight = 106;

        [Description("Listbox의 높이를 지정합니다."), Category("기타"), Browsable(true)]
        public int DropDownHeight
        {
            get { return mTreeViewHeight; }
            set { mTreeViewHeight = value; }
        }

        [Description("컨트롤에 연결된 텍스트입니다."), Category("기타"), Browsable(true), DefaultValue((string)null)]
        public new string Text
        {
            get { return txtMain.Text; }
            set { txtMain.Text = value; }
        }

        [Description("노드 이미지를 가져올 ImageList 컨트롤입니다."), Category("기타"), Browsable(true), DefaultValue((string)null), RefreshProperties(RefreshProperties.Repaint)]
        public ImageList ImageList
        {
            get { return trvMain.ImageList; }
            set { trvMain.ImageList = value; Invalidate(); }
        }

        [Description("노드의 기본 이미지 인텍스입니다."), Category("기타"), RefreshProperties(RefreshProperties.Repaint), TypeConverter(typeof(ImageIndexConverter)),
         Editor("System.Windows.Forms.Design.ImageIndexEditor, System.Design, Version=2.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a", typeof(UITypeEditor)), DefaultValue(-1)]
        public int ImageIndex
        {
            get { return trvMain.ImageIndex; }
            set { trvMain.ImageIndex = value; Invalidate(); }
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public TreeNodeCollection Nodes
        {
            get { return this.trvMain.Nodes; }
        }

        public UserControlComboTreeBox()
        {
            InitializeComponent();
            this.Width = txtMain.Width;
            this.Height = CONST_HEIGHT;
            mControlHeight = CONST_HEIGHT;         
        }

        private void txtMain_SizeChanged(object sender, EventArgs e)
        {
            trvMain.Width = txtMain.Width;
        }

        private void btnDropDown_Click(object sender, EventArgs e)
        {
            if (isDropDown)
            {
                isDropDown = false;
                mControlHeight = CONST_HEIGHT;
            }
            else
            {
                isDropDown = true;
                mControlHeight = CONST_HEIGHT + mTreeViewHeight;
            }

            trvMain.Visible = isDropDown;
            trvMain.Height = mTreeViewHeight;
            this.Height = mControlHeight;

            if (isDropDown == false && txtMain.Text.Trim() != "")
            {
                if (SelectedChanged != null) SelectedChanged(this, e);
            }
        }

        private void ChangeDropDown()
        {

        }

        private void ComboTreeBox_Paint(object sender, PaintEventArgs e)
        {
            trvMain.ExpandAll();
        }

        
        private void trvMain_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
           txtMain.Text = e.Node.Text;
            btnDropDown_Click(this, new EventArgs());
        }

        private void txtMain_TextChanged(object sender, EventArgs e)
        {
            if (EditChanging != null) EditChanging(this, e);
        }

        private void ComboTreeBox_SizeChanged(object sender, EventArgs e)
        {
            txtMain.Width = this.Width;
            btnDropDown.Location = new Point(txtMain.Width - 20, 2);
            this.Height = mControlHeight;
        }

        private void trvMain_Leave(object sender, EventArgs e)
        {
            
        }

        private void ComboTreeBox_Leave(object sender, EventArgs e)
        {
            isDropDown = false;
            mControlHeight = CONST_HEIGHT;

            trvMain.Visible = isDropDown;
            trvMain.Height = mTreeViewHeight;
            this.Height = mControlHeight;
        }

        
    }
}
