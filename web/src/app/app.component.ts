import {Component, OnInit} from '@angular/core';
import {RemotingService} from "./remoting/remoting.service";
import {Title} from "@angular/platform-browser";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  title = 'web';

  constructor(private remoting: RemotingService, private titleService: Title) {
    titleService.setTitle('Talking Clock');
  }

  ngOnInit(): void {
  }
}
