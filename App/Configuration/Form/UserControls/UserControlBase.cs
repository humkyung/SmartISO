using System.Windows.Forms;
//using Configuration.BSL.Entities;
using TechSun.Theme;

namespace Configuration
{
    public partial class UserControlBase : KUserControl , IControl
    {
        //public EntityTagNode _entityTagNode;
        //public EntityEDBInformation _entityEDBInformation;
        public UserControlBase()
        {
            InitializeComponent();
        }

        public virtual void SaveData()
        {

        }

        public virtual void ExportToExcel()
        {

        }

        public virtual void LoadExcel()
        {

        }

        public virtual void LoadData()
        {

        }

        public void InitializeForm()
        {
        }

        public virtual void InitializeControl()
        {

        }

        public Control AsUserControl()
        {
            if (this != null)
                return this as Control;
            else
                return null;
        }
    }
}
