import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-value-display',
  standalone: true,
  imports: [],
  templateUrl: './value-display.component.html',
  styleUrl: './value-display.component.scss'
})
export class ValueDisplayComponent {
  @Input() public labelText = '';
  @Input() public valueText = '';
}
