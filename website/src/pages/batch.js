import React from "react"
import Layout from "../components/Layout"

export default function Home() {
  return (
    <Layout>
        <h3 style={{marginTop: 0}}>How can i run SilentEye from command line? (batch mode)</h3>

        <h4>1. Requirements</h4>
        Batch mode is <b>only available from version 0.4.3</b> of SilentEye. (source <a href="https://github.com/achorein/silenteye">here</a> on branch 0.4)<br/>
        <br/>
        You can change encode and decode options by editing the following files : 
        <ul>
            <li>silenteye.conf</li>
            <li>seformatbmp.conf</li>
            <li>seformatjpeg.conf</li>
            <li>seformatwav.conf</li>
        </ul>
        <i>Configuration files can be found into user's home or SilentEye directory</i>

        <h4>2. Examples</h4>

        Basics:
        <p class="code">
            ./silenteye --batch encode ./myfile.png --message "my secret message"<br/>
            ./silenteye --batch encode ./myfile.png --message_file ./message.txt 
        </p>
        <i>output format and directory depends on silenteye.conf</i><br/>
        <br/>
        Encryption:
        <p class="code">
            ./silenteye --batch encode ./myfile.png --message "my secret message" --password "my password"
        </p>
    </Layout>
  )
}