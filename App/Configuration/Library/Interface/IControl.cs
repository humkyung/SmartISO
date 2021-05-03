using System.ComponentModel;
using System.Windows.Forms;

namespace Configuration
{
    public interface IControl : IComponent
    {
        Control AsUserControl();
    }
}
