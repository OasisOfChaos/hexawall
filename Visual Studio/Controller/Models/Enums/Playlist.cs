namespace HexawallController.Models.Enums
{
    public static class Playlist
    {
        public static List<string> Playlists => new(new string[] { "None", "All of Them", "Nice Flows", "All of Them (but short)" });

        public static string PlaylistName(int index) => Playlists[index];
        public static string PlaylistName(string index) => Playlists[int.Parse(index)];
    }
}
