<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/OasisOfChaos/hexawall">
    <img src="https://github.com/OasisOfChaos/hexawall/blob/main/Images/title.png" alt="Logo">
  </a>

<h3 align="center">HexaWall</h3>

  <p align="center">
    <a href="https://github.com/OasisOfChaos/hexawall">GitHub page</a>
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

### A pretty big ESP32-driven LED thing for on your wall with about 60 'sequences' you can control via MQTT. 

[![Product Name Screen Shot][product-screenshot]](https://github.com/OasisOfChaos/hexawall)

The dimensions of the thing in this pic are about 1.7m by 0.7m but you can make it as large or small as you'd like.

This repo contains all the different projects to make this thing yourself:
* (PlatformIO) ESP32 firmware (this is a framework with plugin-based 'sequences')
* (Autodesk Inventor) 3D files to print the hexagons
* (Eagle) PCB files
* (Dotnet-Core 6 Blazor) website to control the thing

### Some remarks
* Every hexagon has 30 LEDS.
* I used 12VDC WS2815B ledstrips (BTF-Lighting) but because I used the FastLED library you can use a lot of other addressable strips. 
* I soldered them with separate wires but I'd advice to use the solderless connectors instead. Much easier. You may have to enlarge the hexagon a bit for this though.
* Although the PCB has a provision for powering the ESP I could not get that part to work. Maybe a timing problem or just the power supply I used (a 12V16A supply from AliExpress). In the end I just used a separate power adapter to power the ESP via USB.
* Power the LEDS from TWO sides with a wire large enough for 12VDC/8A. I used 2.5mm2 but it can probably be a bit smaller.
* Some of the sequences are made for this layout specifically. You can follow this one or just remove/rewrite the appropriate sequences.
* The hexagon 'covers' are 3mm acrylate sheets, laser cut.
* Yes, I drilled holes in my wall for the wires and control stuff.

### All this is provided as-is. I will not be able to contribute to this project in the future. Use what you like!


<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo_name.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo_name/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo_name.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo_name/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo_name.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo_name/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: https://github.com/OasisOfChaos/hexawall/blob/main/Images/demo.gif
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
