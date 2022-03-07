import React from 'react'
import { Link, withPrefix } from 'gatsby'

export default function Layout({children}) {
    return (
        <>
            <div id="art-page-background-gradient"></div>
            <div id="art-page-background-glare">
                <div id="art-page-background-glare-image"></div>
            </div>
            <div id="art-main">
		<div class="art-sheet">
			<div class="art-sheet-tl"></div>
			<div class="art-sheet-tr"></div>
			<div class="art-sheet-bl"></div>
			<div class="art-sheet-br"></div>
			<div class="art-sheet-tc"></div>
			<div class="art-sheet-bc"></div>
			<div class="art-sheet-cl"></div>
			<div class="art-sheet-cr"></div>
			<div class="art-sheet-cc"></div>
			<div class="art-sheet-body">
				<div class="art-header">
					<div class="art-header-png"></div>
					<div class="art-header-jpeg"></div>
					<div class="art-logo">
						<h1 id="name-text" class="art-logo-name"><a href="#">SilentEye</a></h1>
						<div id="slogan-text" class="art-logo-text">Steganography is yours</div>
					</div>
				</div>
				<div class="art-content-layout">
					<div class="art-content-layout-row">
						<div class="art-layout-cell art-sidebar1">
							<div class="art-vmenublock">
								<div class="art-vmenublock-body">
									<div class="art-vmenublockcontent">
										<div class="art-vmenublockcontent-body">
											<ul class="art-vmenu">
												<li>
                                                    <Link to="/?i1si"><span class="l"></span><span class="r"></span><span class="t">Overview</span></Link>
													<ul>
														<li><Link to="/?i1s1">What is SilentEye?</Link></li>
														<li><Link to="/screenshot/?i1s3">Screenshots</Link></li>
														<li><Link to="/embedding/?i1s2">How does it work?</Link></li>
														<li><Link to="/changes/?i1s3">Changes since the last release</Link></li>
													</ul>
												</li>
												<li>
													<Link to="/download/?i2"><span class="l"></span><span class="r"></span><span class="t">Download</span></Link>
												</li>
												<li>
													<a href="#"><span class="l"></span><span class="r"></span><span class="t">Documentation</span></a>
													<ul>
														<li><Link to="/batch/?i3s1">Run from command line</Link></li>
                                                        <li><Link to="/devenv/?i3s2">Compile source code</Link></li>
														<li><Link to="/writeplugins/?i3s3">Write my own plugin</Link></li>
														<li><a href={withPrefix('/doc/api/lastest')}>API Documentation</a></li>
													</ul>
												</li>
												<li>
													<Link to="/faq/?i4"><span class="l"></span><span class="r"></span><span class="t">FAQ</span></Link>
												</li>
												<li>
													<Link to="/roadmap/?i5"><span class="l"></span><span class="r"></span><span class="t">Roadmap</span></Link>
												</li>
												<li>
													<Link to="/about/?i6"><span class="l"></span><span class="r"></span><span class="t">About</span></Link>
												</li>
											</ul>
											<div class="cleared"></div>
										</div>
									</div>
									<div class="cleared"></div>
								</div>
							</div>
							<div class="art-block">
								<div class="art-blockheader">
									<div class="l"></div>
									<div class="r"></div>
									<div class="t">Bug tracker</div>
								</div>
								<div class="art-blockcontent">
									<div class="art-blockcontent-body">
										<div>
											Feel free to report bugs or ask for new feactures on our <a href="https://github.com/achorein/silenteye/issues"><b>project tracker</b></a>.<br/>
											<i>(quick registration required)</i>
										</div>
										<div class="cleared"></div>
									</div>
								</div>
								<div class="cleared"></div>
							</div>
							<div class="art-block">
								<div class="art-blockheader">
									<div class="l"></div>
									<div class="r"></div>
									<div class="t">News</div>
								</div>
								<div class="art-blockcontent">
									<div class="art-blockcontent-body">
										<div>
											Follow progress on :
											<ul>
												<li>
													<a href="http://twitter.com/_SilentEye" title="Follow us on Twitter" target="_blank">Twitter <img class="image" style={{verticalAlign: 'middle'}} src={withPrefix('/images/social/twitter.png')} alt="Follow us on Twitter"/></a>
												</li>
												<li>
													<a href="http://www.facebook.com/silenteye.steganography" target="_blank">SilentEye's page 
													<img class="image" style={{verticalAlign: 'middle'}} src={withPrefix('images/social/facebook.png')} alt="Post to Facebook"/></a>
<div class="fb-like" data-href="http://www.facebook.com/silenteye.steganography" data-send="false" data-layout="box_count" data-width="160" data-show-faces="false"></div>
												</li>
											</ul>
										</div>
										<div class="cleared"></div>
									</div>
								</div>
								<div class="cleared"></div>
							</div>
						</div>
						<div class="art-layout-cell art-content">
							<div class="art-post">
								<div class="art-post-body">
									<div class="art-post-inner art-article">
										<div class="art-postcontent">
            {children}
            </div>
										<div class="cleared"></div>
									</div>
									<div class="cleared"></div>
								</div>
							</div>
						</div>
						<div class="art-layout-cell art-sidebar2">
							<div class="art-block">
								<div class="art-blockheader">
									<div class="l"></div>
									<div class="r"></div>
									<div class="t">Ads</div>
								</div>
								<div class="art-blockcontent">
									<div class="art-blockcontent-body">
										<script async src="//pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"></script>
										{/* silenteye-right */}
										<ins class="adsbygoogle"
											style={{display:'inline-block', width: 120, height: 600}}
											data-ad-client="ca-pub-5490026576622469"
											data-ad-slot="9158865949"></ins>
										<script>
											(adsbygoogle = window.adsbygoogle || []).push({});
										</script>
										<div class="cleared"></div>
									</div>
								</div>
								<div class="cleared"></div>
							</div>
                        </div>
					</div>
				</div>
				<div class="cleared"></div>
				<div class="art-footer">
					<div class="art-footer-inner">
						<div class="art-footer-text">
							<p>
								<a href="mailto:anselme@chorein.fr">Contact Us</a> | 
								<a href={withPrefix('/licence/COPYING.txt')}>Terms of Use</a> | 
								<a href="http://twitter.com/_SilentEye" title="Follow us on Twitter" target="_blank"><img class="image" src={withPrefix('/images/social/twitter.png')} alt="Follow us on Twitter"/></a>
								<a href="http://delicious.com/post?url=http://www.silenteye.org&amp;title=Steganography+is+yours!" title="Post to Delicious" target="_blank"><img class="image" src={withPrefix('/images/social/delicious.png')} alt="Post to Delicious"/></a>
								<a href="http://digg.com/submit?url=http://www.silenteye.org&amp;title=Steganography+is+yours!" title="Post to Digg" target="_blank"><img class="image" src={withPrefix('/images/social/digg.png')} alt="Post to Digg"/></a>
								<a href="http://www.facebook.com/silenteye.steganography" title="Follow us on our facebook page" target="_blank"><img class="image" src={withPrefix('/images/social/facebook.png')} alt="Follow us on our facebook page"/></a>
								<a href="http://stumbleupon.com/submit?url=http://www.silenteye.org&amp;title=Steganography+is+yours!" title="Post to StumbleUpon" target="_blank"><img class="image" src={withPrefix('/images/social/stumble.png')} alt="Post to StumbleUpon"/></a>
								<br/>Copyright &copy; 2010. All Rights Reserved.
							</p>
						</div>
					</div>
					<div class="art-footer-background"></div>
				</div>
				<div class="cleared"></div>
			</div>
		</div>
		<div class="cleared"></div>
		<p class="art-page-footer"></p>
	</div>
        </>
    )
}
