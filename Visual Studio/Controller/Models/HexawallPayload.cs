namespace HexawallController.Models
{
    public class HexawallPayload
    {
        public bool PowerState { get; set; }
        public int Brightness { get; set; }
        public string ColorMode { get; set; }
        public int ColorTemp { get; set; }
        public RgbwColor Color { get; set; }
        public string Effect { get; set; }
        public int Transition { get; set; }
        public List<int> LedState { get; set; }
    }
}
